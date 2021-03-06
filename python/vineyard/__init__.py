#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 Alibaba Group Holding Limited.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

__version__ = '1.0'

from ._C import connect, IPCClient, RPCClient, Object, ObjectBuilder, ObjectID, ObjectMeta, \
    InstanceStatus, Blob, BlobBuilder, Buffer, MutableBuffer
from . import _vineyard_docs
del _vineyard_docs

from .core import default_builder_context, default_resolver_context, default_driver_context
from .data import register_builtin_types
from .data.base import ObjectSet
from .data.graph import Graph


def _init_vineyard_modules():
    ''' Resolve registered vineyard modules in the following order:

        * /etc/vineyard/config.py
        * {sys.prefix}/etc/vineyard/config.py
        * /usr/local/share/vineyard/01-xxx.py
        * {sys.prefix}/share/vineyard/02-xxxx.py
        * $HOME/.vineyard/03-xxxxx.py
    '''

    import glob
    import importlib.util
    import os
    import sys

    def _import_module_from_file(filepath):
        if os.path.exists(filepath):
            try:
                spec = importlib.util.spec_from_file_location("vineyard._contrib", filepath)
                mod = importlib.util.module_from_spec(spec)
                spec.loader.exec_module(mod)
            except:
                pass

    _import_module_from_file('/etc/vineyard/config.py')
    _import_module_from_file(os.path.join(sys.prefix, '/etc/vineyard/config.py'))
    for filepath in glob.glob('/usr/local/share/vineyard/*-*.py'):
        _import_module_from_file(filepath)
    for filepath in glob.glob(os.path.join(sys.prefix, '/share/vineyard/*-*.py')):
        _import_module_from_file(filepath)
    for filepath in glob.glob(os.path.expanduser('$HOME/.vineyard/*-*.py')):
        _import_module_from_file(filepath)


try:
    _init_vineyard_modules()
except:
    pass
del _init_vineyard_modules
