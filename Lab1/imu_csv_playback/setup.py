from setuptools import find_packages, setup
import os, csv
from glob import glob

package_name = 'imu_csv_playback'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/data',
            ['data/imu_data.csv']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='seif',
    maintainer_email='seif924@hotmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'imu_playback = imu_csv_playback.imu_csv_playback:main',
        ],
    },
)
