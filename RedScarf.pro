#-------------------------------------------------
#
# Project created by Eduardo Alves da Silva
#
#-------------------------------------------------
#
# RedScarf.pro
# Copyright (C) 2014-2015 LEDS - Univali <zeferino@univali.br>
# Laboratory of Embedded and Distributed Systems
# University of Vale do Itajaí
#
# Authors: Laboratory of Embedded and Distributed Systems (LEDS)
#
# Contact: Profº. Cesar Albenes Zeferino, PhD {zeferino@univali.br}
#
# RedScarf is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RedScarf is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ------------------------------ Reviews -------------------------------------
# Date       - Version - Author                      | Description
# ----------------------------------------------------------------------------
# 13/06/2015 - 1.0     - Eduardo Alves da Silva      | Initial release
#
#

TEMPLATE = subdirs
SUBDIRS = RS \
          RS/test \
          RS/dist \
          RS/bin/system
