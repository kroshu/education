from setuptools import find_packages, setup
import os
from glob import glob

package_name = 'service_example'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob(os.path.join('launch', '*launch.py'))),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Kiss Anna',
    maintainer_email='kiss.anna203@gmail.com',
    description='Example to showcase the usage of services',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'server = service_example.get_age_server:main',
            'client = service_example.get_age_client:main',
        ],
    },
)
