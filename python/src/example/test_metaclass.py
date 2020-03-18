#!/usr/bin/python3

class Field(object):  # 属性域
    def __init__(self, name, column_type):
        self.name = name
        self.column_type = column_type

    def __str__(self):
        return '<%s:%s>' % (self.__class__.__name__, self.name)

class StringField(Field): # 字符串属性
    
    def __init__(self, name):
        super(StringField, self).__init__(name, 'varchar(100)')

class IntegerField(Field):  # 整数属性

    def __init__(self, name):
        super(IntegerField, self).__init__(name, 'bigint')

class ModelMetaclass(type):
    
    def __new__(cls, name, bases, attrs):
        if name=='Model': # 如果是model类型就不做处理
            return type.__new__(cls, name, bases, attrs)
        print('Found model: %s' % name)  # 打印当前实际生成的类名
        mappings = dict()  # 用于存储当前类的属性名与属性之间的映射
        for k, v in attrs.items():   # 遍历所有属性名
            if isinstance(v, Field):   # 如果是已知的属性域
                print('Found mapping: %s ==> %s' % (k, v))
                mappings[k] = v    # 加入映射
        for k in mappings.keys():
            attrs.pop(k)     # 从原有属性中删除
        attrs['__mappings__'] = mappings # 保存属性和列的映射关系
        attrs['__table__'] = name # 假设表名和类名一致
        return type.__new__(cls, name, bases, attrs)

class Model(dict, metaclass=ModelMetaclass):
    
    def __init__(self, **kw):
        super(Model, self).__init__(**kw)

    def __getattr__(self, key):
        try:
            return self[key]
        except KeyError:
            raise AttributeError(r"'Model' object has no attribute '%s'" % key)

    def __setattr__(self, key, value):
        self[key] = value

    def save(self):
        fields = []
        params = []
        args = []
        for k, v in self.__mappings__.items():
            fields.append(v.name)
            params.append('?')
            args.append(getattr(self, k, None))
        sql = 'insert into %s (%s) values (%s)' % (self.__table__, ','.join(fields), ','.join(params))
        print('SQL: %s' % sql)
        print('ARGS: %s' % str(args))
    
    def say_mapping(self):
        for k,v in self.__mappings__.items():
            print(k,'->',v)
    
    def say_table(self):
        print(self.__table__) 

class User(Model):
    # 定义类的属性到列的映射：
    id = IntegerField('id')
    name = StringField('username')
    email = StringField('email')
    password = StringField('password')

u = User(id=12345, name='Michael', email='test@orm.org', password='my-pwd')
u.save()

print()

u.say_mapping()

print()

u.say_table()