<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class MunicipiosModel extends Model{
	private $municipios;

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

	public function ListarMunicipios($uf=null){
		$municipios = array();
		if (isset($uf)) {
			$sql = "SELECT
				  		M.co_municipio AS codigo,
				  		M.no_municipio AS municipio,
				  		M.sg_estado AS estado
					FROM
				  		tb_reuni_municipio AS M
					WHERE
				  		sg_estado='$uf'
					ORDER BY
				  		no_municipio";
		if ($this->dao->fetch($sql)) {
			while ($municipio = $this->dao->getRow()) {
					$this->municipios[]=$municipio;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
		}
	}

    public function getMunicipios() {
        return $this->municipios;
    }
}

?>
