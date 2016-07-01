<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class CusteioModel extends Model{
	private $custeio;
	private $totalcusteio;
	private $totalcreditoscusteio;
	private $totalpeqexpansao;

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

	public function AtualizarCusteio($codigo,$ano,$valor) {
		$sql = "UPDATE tb_reuni_orcamento
				SET vl_orcamento = ".$valor."
				WHERE co_instituicao=".$this->instituicao." AND
			    co_orcamento=".$codigo." AND
			    nu_ano=".$ano;
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	private function ListarCreditosCusteio() {
	$sql = "SELECT
	co_instituicao,
	SUM(vl_total_2008) AS vl_total_2008,
	SUM(vl_total_2009) AS vl_total_2009,
	SUM(vl_total_2010) AS vl_total_2010,
	SUM(vl_total_2011) AS vl_total_2011,
	SUM(vl_total_2012) AS vl_total_2012
	FROM
	(
	SELECT
		co_instituicao,
 		vl_credito AS vl_total_2008,
 		0 AS vl_total_2009,
 		0 AS vl_total_2010,
 		0 AS vl_total_2011,
 		0 AS vl_total_2012
	FROM
 		tb_reuni_credito
	WHERE
		nu_ano=2008 AND
		tp_credito='C' AND
 		co_instituicao=$this->instituicao

	UNION

	SELECT
		co_instituicao,
 		0 AS vl_total_2008,
 		vl_credito AS vl_total_2009,
 		0 AS vl_total_2010,
 		0 AS vl_total_2011,
 		0 AS vl_total_2012
	FROM
 		tb_reuni_credito
	WHERE
		nu_ano=2009 AND
		tp_credito='C' AND
 		co_instituicao=$this->instituicao

	UNION

	SELECT
		co_instituicao,
 		0 AS vl_total_2008,
 		0 AS vl_total_2009,
 		vl_credito AS vl_total_2010,
 		0 AS vl_total_2011,
 		0 AS vl_total_2012
	FROM
 		tb_reuni_credito
	WHERE
		nu_ano=2010 AND
		tp_credito='C' AND
 		co_instituicao=$this->instituicao

	UNION

	SELECT
		co_instituicao,
 		0 AS vl_total_2008,
 		0 AS vl_total_2009,
 		0 AS vl_total_2010,
 		vl_credito AS vl_total_2011,
 		0 AS vl_total_2012
	FROM
 		tb_reuni_credito
	WHERE
		nu_ano=2011 AND
		tp_credito='C' AND
 		co_instituicao=$this->instituicao

	UNION

	SELECT
		co_instituicao,
 		0 AS vl_total_2008,
 		0 AS vl_total_2009,
 		0 AS vl_total_2010,
 		0 AS vl_total_2011,
 		vl_credito AS vl_total_2012
	FROM
 		tb_reuni_credito
	WHERE
		nu_ano=2012 AND
		tp_credito='C' AND
 		co_instituicao=$this->instituicao

) AS CONS
GROUP BY
	co_instituicao";
		if ($this->dao->fetch($sql)) {
			$this->totalcreditoscusteio = $this->dao->getRow();
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarCusteio(){
		$this->ListarCreditosCusteio();
		$this->ListarPeqExpansao();
		$custeio = array();
		$sql = "SELECT
 					co_instituicao,
 					co_orcamento,
 					ds_orcamento,
 					vl_unidade,
 					SUM(unidades_2008) AS unidades_2008,
 					SUM(vl_total_2008) AS vl_total_2008,
 					SUM(unidades_2009) AS unidades_2009,
 					SUM(vl_total_2009) AS vl_total_2009,
 					SUM(unidades_2010) AS unidades_2010,
 					SUM(vl_total_2010) AS vl_total_2010,
 					SUM(unidades_2011) AS unidades_2011,
 					SUM(vl_total_2011) AS vl_total_2011,
 					SUM(unidades_2012) AS unidades_2012,
 					SUM(vl_total_2012) AS vl_total_2012
				FROM
				(
				SELECT
 					O.co_instituicao,
 					O.co_orcamento,
 					T.ds_orcamento,
 					T.vl_unidade,
 					O.vl_orcamento AS unidades_2008,
 					O.vl_orcamento*T.vl_unidade AS vl_total_2008,
 					0 AS unidades_2009,
 					0 AS vl_total_2009,
 					0 AS unidades_2010,
 					0 AS vl_total_2010,
 					0 AS unidades_2011,
 					0 AS vl_total_2011,
 					0 AS unidades_2012,
 					0 AS vl_total_2012
				FROM
 					tb_reuni_orcamento AS O,
 					tb_reuni_tipo_orcamento AS T
				WHERE
 					T.co_orcamento=O.co_orcamento AND
 					nu_ano=2008 AND
 					tp_orcamento='C' AND
 					O.co_instituicao=$this->instituicao

				UNION

				SELECT
 					O.co_instituicao,
 					O.co_orcamento,
 					T.ds_orcamento,
 					T.vl_unidade,
 					0 AS unidades_2008,
 					0 AS vl_total_2008,
 					O.vl_orcamento AS unidades_2009,
 					O.vl_orcamento*T.vl_unidade AS vl_total_2009,
 					0 AS unidades_2010,
 					0 AS vl_total_2010,
 					0 AS unidades_2011,
 					0 AS vl_total_2011,
 					0 AS unidades_2012,
 					0 AS vl_total_2012
				FROM
 					tb_reuni_orcamento AS O,
 					tb_reuni_tipo_orcamento AS T
				WHERE
 					T.co_orcamento=O.co_orcamento AND
 					nu_ano=2009 AND
 					tp_orcamento='C' AND
 					O.co_instituicao=$this->instituicao

				UNION

				SELECT
 					O.co_instituicao,
 					O.co_orcamento,
 					T.ds_orcamento,
 					T.vl_unidade,
 					0 AS unidades_2008,
 					0 AS vl_total_2008,
 					0 AS unidades_2009,
 					0 AS vl_total_2009,
 					O.vl_orcamento AS unidades_2010,
 					O.vl_orcamento*T.vl_unidade AS vl_total_2010,
 					0 AS unidades_2011,
 					0 AS vl_total_2011,
 					0 AS unidades_2012,
 					0 AS vl_total_2012
				FROM
 					tb_reuni_orcamento AS O,
 					tb_reuni_tipo_orcamento AS T
				WHERE
 					T.co_orcamento=O.co_orcamento AND
 					nu_ano=2010 AND
 					tp_orcamento='C' AND
 					O.co_instituicao=$this->instituicao

				UNION

				SELECT
 					O.co_instituicao,
 					O.co_orcamento,
 					T.ds_orcamento,
					T.vl_unidade,
 					0 AS unidades_2008,
 					0 AS vl_total_2008,
 					0 AS unidades_2009,
 					0 AS vl_total_2009,
 					0 AS unidades_2010,
 					0 AS vl_total_2010,
 					O.vl_orcamento AS unidades_2011,
 					O.vl_orcamento*T.vl_unidade AS vl_total_2011,
 					0 AS unidades_2012,
 					0 AS vl_total_2012
				FROM
 					tb_reuni_orcamento AS O,
 					tb_reuni_tipo_orcamento AS T
				WHERE
 					T.co_orcamento=O.co_orcamento AND
 					nu_ano=2011 AND
 					tp_orcamento='C' AND
 					O.co_instituicao=$this->instituicao

				UNION

				SELECT
 					O.co_instituicao,
 					O.co_orcamento,
 					T.ds_orcamento,
					T.vl_unidade,
 					0 AS unidades_2008,
 					0 AS vl_total_2008,
 					0 AS unidades_2009,
 					0 AS vl_total_2009,
 					0 AS unidades_2010,
 					0 AS vl_total_2010,
 					0 AS unidades_2011,
 					0 AS vl_total_2011,
 					O.vl_orcamento AS unidades_2012,
	 				O.vl_orcamento*T.vl_unidade AS vl_total_2012
				FROM
 					tb_reuni_orcamento AS O,
 					tb_reuni_tipo_orcamento AS T
				WHERE
 					T.co_orcamento=O.co_orcamento AND
 					nu_ano=2012 AND
 					tp_orcamento='C' AND
 					O.co_instituicao=$this->instituicao

				) AS CONS
				GROUP BY
	 				co_instituicao,
 					co_orcamento,
 					ds_orcamento,
 					vl_unidade";
		if ($this->dao->fetch($sql)) {
			while ($c = $this->dao->getRow()) {
				$this->totalcusteio['2008']+=$c['vl_total_2008'];
				$this->totalcusteio['2009']+=$c['vl_total_2009'];
				$this->totalcusteio['2010']+=$c['vl_total_2010'];
				$this->totalcusteio['2011']+=$c['vl_total_2011'];
				$this->totalcusteio['2012']+=$c['vl_total_2012'];
				$this->custeio[]=$c;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	private function ListarPeqExpansao() {
	$sql = "select
				   a.vl_peq_2007 +
				   (select coalesce(sum(y.vl_peq_expansao),0) from tb_reuni_peq_expansao y where y.co_instituicao = a.co_instituicao  and y.nu_ano <= 2007)
				   as vl_peq_2007,

				   a.vl_peq_2007 +
				   (select coalesce(sum(x.vl_orcamento*1.55),0) from tb_reuni_orcamento x where x.co_instituicao = a.co_instituicao and x.co_orcamento = 6 and x.nu_ano = 2008) +
				   (select coalesce(sum(y.vl_peq_expansao),0) from tb_reuni_peq_expansao y where y.co_instituicao = a.co_instituicao  and y.nu_ano <= 2008)
				   as vl_peq_2008,

				   a.vl_peq_2007 +
				   (select coalesce(sum(x.vl_orcamento*1.55),0) from tb_reuni_orcamento x where x.co_instituicao = a.co_instituicao and x.co_orcamento = 6 and x.nu_ano = 2009) +
				   (select coalesce(sum(y.vl_peq_expansao),0) from tb_reuni_peq_expansao y where y.co_instituicao = a.co_instituicao  and y.nu_ano <= 2009)
				   as vl_peq_2009,

				   a.vl_peq_2007 +
				   (select coalesce(sum(x.vl_orcamento*1.55),0) from tb_reuni_orcamento x where x.co_instituicao = a.co_instituicao and x.co_orcamento = 6 and x.nu_ano = 2010) +
				   (select coalesce(sum(y.vl_peq_expansao),0) from tb_reuni_peq_expansao y where y.co_instituicao = a.co_instituicao  and y.nu_ano <= 2010)
				   as vl_peq_2010,

				   a.vl_peq_2007 +
				   (select coalesce(sum(x.vl_orcamento*1.55),0) from tb_reuni_orcamento x where x.co_instituicao = a.co_instituicao and x.co_orcamento = 6 and x.nu_ano = 2011) +
				   (select coalesce(sum(y.vl_peq_expansao),0) from tb_reuni_peq_expansao y where y.co_instituicao = a.co_instituicao  and y.nu_ano <= 2011)
				   as vl_peq_2011,

				   a.vl_peq_2007 +
				   (select coalesce(sum(x.vl_orcamento*1.55),0) from tb_reuni_orcamento x where x.co_instituicao = a.co_instituicao and x.co_orcamento = 6 and x.nu_ano = 2012) +
				   (select coalesce(sum(y.vl_peq_expansao),0) from tb_reuni_peq_expansao y where y.co_instituicao = a.co_instituicao  and y.nu_ano <= 2012)
				   as vl_peq_2012,

				   a.vl_peq_2007 +
				   (select coalesce(sum(x.vl_orcamento*1.55),0) from tb_reuni_orcamento x where x.co_instituicao = a.co_instituicao and x.co_orcamento = 6 and x.nu_ano = 2012) +
				   (select coalesce(sum(y.vl_peq_expansao),0) from tb_reuni_peq_expansao y where y.co_instituicao = a.co_instituicao  and y.nu_ano <= 2012)
				   as vl_peq_2017
				from
				   tb_reuni_instituicao a
				where
				   a.co_instituicao = $this->instituicao
			";
		if ($this->dao->fetch($sql)) {
			$this->totalpeqexpansao = $this->dao->getRow();
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

    public function getCusteio() {
        return $this->custeio;
    }

    public function getTotalCusteio() {
        return $this->totalcusteio;
    }

    public function getTotalCreditos() {
        return $this->totalcreditoscusteio;
    }

    public function getTotalPeqExpansao() {
        return $this->totalpeqexpansao;
    }

}

?>
