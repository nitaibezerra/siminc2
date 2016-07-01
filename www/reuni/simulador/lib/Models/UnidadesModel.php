<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class UnidadesModel extends Model{
	private $unidades;
	private $estados;
	private $municipios;

	public  $action;
	public  $unidade;

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

	public function ListarEstados(){
		$this->estados = array();
		$sql = "SELECT DISTINCT sg_estado FROM tb_reuni_municipio ORDER BY sg_estado";
		if ($this->dao->fetch($sql)) {
			while ($estado = $this->dao->getRow()) {
				$this->estados[]=$estado;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarMunicipios($uf=null){
		$this->municipios = array();
		$sql = "SELECT DISTINCT co_municipio,no_municipio FROM tb_reuni_municipio WHERE sg_estado='".$uf."' ORDER BY no_municipio";
		if ($this->dao->fetch($sql)) {
			while ($municipio = $this->dao->getRow()) {
				$this->municipios[]=$municipio;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function InserirUnidade($nome,$municipio){
		$sql = "INSERT INTO tb_reuni_unidade (co_municipio,co_instituicao,no_unidade)
				VALUES ('$municipio',".$this->instituicao.",'$nome')";
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function AlterarUnidade($unidade,$nome,$municipio){
		$sql = "UPDATE tb_reuni_unidade SET co_municipio = '$municipio', no_unidade = '$nome'
				WHERE co_instituicao = ".$this->instituicao." AND co_unidade = $unidade";
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function RemoverUnidade($unidade){
		$sql = "DELETE FROM tb_reuni_unidade WHERE co_unidade=$unidade";
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function PegarUnidade($codunidade,$unidade,$uf,$cod_municipio,$municipio){
		$sql = "SELECT
				  U.co_unidade,
				  U.no_unidade,
				  M.co_municipio,
				  M.no_municipio,
				  M.sg_estado
				FROM
  				  tb_reuni_unidade AS U,
				  tb_reuni_municipio AS M
				WHERE
				  U.co_municipio=M.co_municipio
				  AND U.co_unidade=$codunidade
				ORDER BY
				  sg_estado,
				  no_municipio,
				  no_unidade";
		if ($this->dao->fetch($sql)) {
			$uni = $this->dao->getRow();
			$unidade=$uni['no_unidade'];
			$cod_municipio=$uni['co_municipio'];
			$municipio=$uni['no_municipio'];
			$uf=$uni['sg_estado'];
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarUnidades(){
		$sql = "SELECT
				  U.co_unidade,
				  U.no_unidade,
				  M.no_municipio,
				  M.sg_estado
				FROM
  				  tb_reuni_unidade AS U,
				  tb_reuni_municipio AS M
				WHERE
				  co_instituicao=".$this->instituicao."
				  AND U.co_municipio=M.co_municipio
				ORDER BY
				  sg_estado,
				  no_municipio,
				  no_unidade";
		if ($this->dao->fetch($sql)) {
			while ($unidade = $this->dao->getRow()) {
				$this->unidades[]=$unidade;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

    public function getUnidades() {
        return $this->unidades;
    }

    public function getEstados() {
        return $this->estados;
    }

    public function getMunicipios() {
        return $this->municipios;
    }
}

?>
