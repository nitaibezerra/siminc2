<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class InvestimentoModel extends Model{
	private $investimento;
	private $totalinvestimento;
	private $totalcreditosinvestiemento;
	private $totalconstrucao;

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

	public function AtualizarInvestimento($codigo,$ano,$valor) {
		$sql = "UPDATE tb_reuni_orcamento
				SET vl_orcamento = ".$valor."
				WHERE co_instituicao = ".$this->instituicao." AND
			    co_orcamento = ".$codigo." AND
			    nu_ano = ".$ano;
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function AtualizarConstrucao($ano,$valor) {
		$sql = "UPDATE tb_reuni_construcao
				SET qt_m2_construcao = ".$valor."
				WHERE co_instituicao = ".$this->instituicao." AND
			    nu_ano = ".$ano;
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarConstrucao() {
		$sql = "SELECT
					co_instituicao,
					SUM(vl_total_2008) AS vl_total_2008,
					SUM(vl_total_2009) AS vl_total_2009,
					SUM(vl_total_2010) AS vl_total_2010,
					SUM(vl_total_2011) AS vl_total_2011
				FROM
				(
				SELECT
					co_instituicao,
			 		qt_m2_construcao AS vl_total_2008,
			 		0 AS vl_total_2009,
			 		0 AS vl_total_2010,
			 		0 AS vl_total_2011
				FROM
			 		tb_reuni_construcao
				WHERE
					nu_ano = 2008 AND
			 		co_instituicao = $this->instituicao

				UNION

				SELECT
					co_instituicao,
			 		0 AS vl_total_2008,
			 		qt_m2_construcao AS vl_total_2009,
			 		0 AS vl_total_2010,
			 		0 AS vl_total_2011
				FROM
			 		tb_reuni_construcao
				WHERE
					nu_ano = 2009 AND
			 		co_instituicao = $this->instituicao

				UNION

				SELECT
					co_instituicao,
			 		0 AS vl_total_2008,
			 		0 AS vl_total_2009,
			 		qt_m2_construcao AS vl_total_2010,
			 		0 AS vl_total_2011
				FROM
			 		tb_reuni_construcao
				WHERE
					nu_ano = 2010 AND
			 		co_instituicao = $this->instituicao

				UNION

				SELECT
					co_instituicao,
			 		0 AS vl_total_2008,
			 		0 AS vl_total_2009,
			 		0 AS vl_total_2010,
			 		qt_m2_construcao AS vl_total_2011
				FROM
			 		tb_reuni_construcao
				WHERE
					nu_ano = 2011 AND
			 		co_instituicao = $this->instituicao

				) AS CONS
				GROUP BY
					co_instituicao
				";
		if ($this->dao->fetch($sql)) {
			$this->totalconstrucao = $this->dao->getRow();
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarCreditosInvestimento() {
		$sql = "SELECT
					co_instituicao,
					SUM(vl_total_2008) AS vl_total_2008,
					SUM(vl_total_2009) AS vl_total_2009,
					SUM(vl_total_2010) AS vl_total_2010,
					SUM(vl_total_2011) AS vl_total_2011
				FROM
				(
				SELECT
					co_instituicao,
			 		vl_credito AS vl_total_2008,
			 		0 AS vl_total_2009,
			 		0 AS vl_total_2010,
			 		0 AS vl_total_2011
				FROM
			 		tb_reuni_credito
				WHERE
					nu_ano = 2008 AND
					tp_credito = 'I' AND
			 		co_instituicao = $this->instituicao

				UNION

				SELECT
					co_instituicao,
			 		0 AS vl_total_2008,
			 		vl_credito AS vl_total_2009,
			 		0 AS vl_total_2010,
			 		0 AS vl_total_2011
				FROM
			 		tb_reuni_credito
				WHERE
					nu_ano = 2009 AND
					tp_credito = 'I' AND
			 		co_instituicao = $this->instituicao

				UNION

				SELECT
					co_instituicao,
			 		0 AS vl_total_2008,
			 		0 AS vl_total_2009,
			 		vl_credito AS vl_total_2010,
			 		0 AS vl_total_2011
				FROM
			 		tb_reuni_credito
				WHERE
					nu_ano = 2010 AND
					tp_credito = 'I' AND
			 		co_instituicao = $this->instituicao

				UNION

				SELECT
					co_instituicao,
			 		0 AS vl_total_2008,
			 		0 AS vl_total_2009,
			 		0 AS vl_total_2010,
			 		vl_credito AS vl_total_2011
				FROM
			 		tb_reuni_credito
				WHERE
					nu_ano = 2011 AND
					tp_credito = 'I' AND
			 		co_instituicao = $this->instituicao

				) AS CONS
				GROUP BY
					co_instituicao
				";
		if ($this->dao->fetch($sql)) {
			$this->totalcreditosinvestimento = $this->dao->getRow();
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}


	public function ListarInvestimento(){
    	$this->ListarCreditosInvestimento();
    	$this->ListarConstrucao();
		$this->investimento = array();
		$this->totalinvestimento = array();
		$this->totalcreditos = array();
		$sql = "SELECT
 				co_instituicao,
 				co_orcamento,
 				ds_orcamento,
 				SUM(vl_total_2008) AS vl_total_2008,
 				SUM(vl_total_2009) AS vl_total_2009,
 				SUM(vl_total_2010) AS vl_total_2010,
 				SUM(vl_total_2011) AS vl_total_2011
				FROM
				(
				SELECT
 				O.co_instituicao,
 				O.co_orcamento,
 				T.ds_orcamento,
 				O.vl_orcamento AS vl_total_2008,
 				0 AS vl_total_2009,
 				0 AS vl_total_2010,
 				0 AS vl_total_2011
				FROM
 				tb_reuni_orcamento AS O,
 				tb_reuni_tipo_orcamento AS T
				WHERE
 				T.co_orcamento = O.co_orcamento AND
 				nu_ano = 2008 AND
 				tp_orcamento = 'I' AND
 				O.co_instituicao = $this->instituicao

				UNION

				SELECT
 				O.co_instituicao,
 				O.co_orcamento,
 				T.ds_orcamento,
 				0 AS vl_total_2008,
 				O.vl_orcamento AS vl_total_2009,
 				0 AS vl_total_2010,
 				0 AS vl_total_2011
				FROM
 				tb_reuni_orcamento AS O,
 				tb_reuni_tipo_orcamento AS T
				WHERE
 				T.co_orcamento = O.co_orcamento AND
 				nu_ano = 2009 AND
 				tp_orcamento = 'I' AND
 				O.co_instituicao = $this->instituicao

				UNION

				SELECT
 				O.co_instituicao,
 				O.co_orcamento,
 				T.ds_orcamento,
 				0 AS vl_total_2008,
 				0 AS vl_total_2009,
 				O.vl_orcamento AS vl_total_2010,
 				0 AS vl_total_2011
				FROM
 				tb_reuni_orcamento AS O,
 				tb_reuni_tipo_orcamento AS T
				WHERE
 				T.co_orcamento = O.co_orcamento AND
 				nu_ano = 2010 AND
 				tp_orcamento = 'I' AND
 				O.co_instituicao = $this->instituicao

				UNION

				SELECT
 				O.co_instituicao,
 				O.co_orcamento,
 				T.ds_orcamento,
 				0 AS vl_total_2008,
 				0 AS vl_total_2009,
 				0 AS vl_total_2010,
 				O.vl_orcamento AS vl_total_2011
				FROM
 				tb_reuni_orcamento AS O,
 				tb_reuni_tipo_orcamento AS T
				WHERE
 				T.co_orcamento = O.co_orcamento AND
 				nu_ano = 2011 AND
 				tp_orcamento = 'I' AND
 				O.co_instituicao = $this->instituicao
				) AS CONS
				GROUP BY
				 co_instituicao,
				 co_orcamento,
				 ds_orcamento
				";
		if ($this->dao->fetch($sql)) {
			while ($c = $this->dao->getRow()) {
				$this->totalinvestimento['2008']+=$c['vl_total_2008'];
				$this->totalinvestimento['2009']+=$c['vl_total_2009'];
				$this->totalinvestimento['2010']+=$c['vl_total_2010'];
				$this->totalinvestimento['2011']+=$c['vl_total_2011'];
				$this->investimento[]=$c;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

    public function getInvestimento() {
        return $this->investimento;
    }

    public function getTotalInvestimento() {
        return $this->totalinvestimento;
    }

    public function getTotalCreditos() {
        return $this->totalcreditosinvestimento;
    }

    public function getConstrucao() {
        return $this->totalconstrucao;
    }
}

?>
