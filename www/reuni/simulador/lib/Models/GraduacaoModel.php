<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class GraduacaoModel extends Model{
	private $areas;
	private $cursos;
	private $vagas;
	private $vagasTCG;
	private $unidades;
	public  $action;
	public  $curso;

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

	public function PegarCursoGrad($codCurso, $co_curso, $co_unidade, $co_inep, $no_curso, $co_turno, $dt_ano_inicio, $co_area, $vl_duracao) {
		$sql = "SELECT
				   co_curso,
				   co_unidade,
				   co_inep,
				   no_curso,
				   co_turno,
				   dt_ano_inicio,
				   co_area,
				   vl_duracao
				FROM
				   tb_reuni_graduacao
				WHERE
				   co_curso = $codCurso
				ORDER BY
				   co_unidade,
				   no_curso,
				   co_turno";
		if ($this->dao->fetch($sql)) {
			$cur = $this->dao->getRow();
			$co_curso      = $cur["co_curso"];
			$co_unidade    = $cur["co_unidade"];
			$co_inep       = $cur["co_inep"];
			$no_curso      = $cur["no_curso"];
			$co_turno      = $cur["co_turno"];
			$dt_ano_inicio = $cur["dt_ano_inicio"];
			$co_area       = $cur["co_area"];
			$vl_duracao    = $cur["vl_duracao"];
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function PegarUnidadeCursoGrad($codUnidade, $unidade, $municipio, $uf) {
		$m = new UnidadesModel($this->dao);
		$u = $m->PegarUnidade($codUnidade,&$unidade,&$uf,&$comunicipio,&$municipio);
		$m = null;
		return $u;
	}

	public function InserirCursoGrad($unidade, $nomeCurso, $turno, $inicio, $area, $duracao){
		$sql = "START TRANSACTION";
		$this->dao->fetch($sql);
		$sql = "INSERT INTO tb_reuni_graduacao (co_unidade, no_curso, co_turno, dt_ano_inicio, co_area, vl_duracao)
				VALUES ($unidade, '$nomeCurso', '$turno', $inicio, '$area', $duracao) RETURNING co_curso";
		$R1 = $this->dao->fetch($sql);
		$RR = $this->dao->getRow();
		$sql = "INSERT INTO tb_reuni_vagas_graduacao VALUES (".$RR['co_curso'].",2006,0,0),(".$RR['co_curso'].",2007,0,0),(".$RR['co_curso'].",2008,0,0),(".$RR['co_curso'].",2009,0,0),(".$RR['co_curso'].",2010,0,0),(".$RR['co_curso'].",2011,0,0),(".$RR['co_curso'].",2012,0,0),(".$RR['co_curso'].",2017,0,0)";
		$R2 = $this->dao->fetch($sql);
		if (($R1==TRUE)AND($R2==TRUE)) {
			$sql = "COMMIT";
			$this->dao->fetch($sql);
			return TRUE;
		} else {
			$erro = "";
			if (!$R1) $erro .= $R1;
			if (!$R2) $erro .= $R2;
			$sql = "ROLLBACK";
			$this->dao->fetch($sql);
			return $erro;
		}
	}

	public function AlterarCursoGrad($codCurso, $unidade, $nomeCurso, $turno, $inicio, $area, $duracao){
		$sql = "START TRANSACTION";
		$this->dao->fetch($sql);
		$sql = "UPDATE tb_reuni_graduacao SET
				    co_unidade    = $unidade,
				    no_curso      = '$nomeCurso',
				    co_turno      = '$turno',
				    dt_ano_inicio = $inicio,
				    co_area       = '$area',
				    vl_duracao    = $duracao
				WHERE
					co_curso = $codCurso";
		$R1 = $this->dao->fetch($sql);
		$sql = "UPDATE tb_reuni_vagas_graduacao
				SET nu_concluintes = 0, nu_vagas = 0
				WHERE co_curso=".$codCurso." AND
			    nu_ano<".$inicio;
		$R2 = $this->dao->fetch($sql);
		if (($R1==TRUE)AND($R2==TRUE)) {
			$sql = "COMMIT";
			$this->dao->fetch($sql);
			return TRUE;
		} else {
			$erro = "";
			if (!$R1) $erro .= $R1;
			if (!$R2) $erro .= $R2;
			$sql = "ROLLBACK";
			$this->dao->fetch($sql);
			return $erro;
		}
	}

	public function RemoverCursoGrad($curso){
		$sql = "DELETE FROM tb_reuni_graduacao WHERE co_curso = $curso";
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarCursosGrad(){
		$this->cursos = array();
		$sql = "SELECT
				   a.co_curso,
				   b.no_unidade,
				   a.co_inep,
				   a.no_curso,
				   a.co_turno,
				   a.dt_ano_inicio,
				   a.co_area,
				   a.vl_duracao
				FROM
				   tb_reuni_graduacao a
				   JOIN tb_reuni_unidade b on b.co_unidade = a.co_unidade
				   JOIN tb_reuni_instituicao c on c.co_instituicao = b.co_instituicao
				WHERE
				   c.co_instituicao = $this->instituicao
				ORDER BY
				   b.no_unidade,
				   a.no_curso,
				   a.co_turno";
		if ($this->dao->fetch($sql)) {
			while ($curso = $this->dao->getRow()) {
				$this->cursos[]=$curso;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarUnidades($inst){
		$this->unidades = array();
		$sql = "SELECT
				   co_unidade,
				   no_unidade
				FROM
				   tb_reuni_unidade
				WHERE
				   co_instituicao = $inst
				ORDER BY
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

	public function ListarAreas(){
		$this->areas = array();
		$sql = "SELECT
				   co_area,
				   ds_area
				FROM
				   tb_reuni_area
				ORDER BY
				   ds_area";
		if ($this->dao->fetch($sql)) {
			while ($area = $this->dao->getRow()) {
				$this->areas[]=$area;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function AtualizarGraduacaoVagas($codCurso,$ano,$vagas) {
		$sql = "UPDATE tb_reuni_vagas_graduacao
				SET nu_vagas = ".$vagas."
				WHERE co_curso=".$codCurso." AND
			    nu_ano=".$ano;
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function AtualizarGraduacaoConcluintes($codCurso,$ano,$concluintes) {
		$sql = "UPDATE tb_reuni_vagas_graduacao
				SET nu_concluintes = ".$concluintes."
				WHERE co_curso=".$codCurso." AND
			    nu_ano=".$ano;
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarGraduacaoVagas($codCurso){
		$this->vagas = array();
		$sql = "SELECT nu_ano,nu_vagas,nu_concluintes FROM tb_reuni_vagas_graduacao
				WHERE co_curso= $codCurso order by nu_ano";
		if ($this->dao->fetch($sql)) {
			while ($c = $this->dao->getRow()) {
				$this->vagas[]=$c;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarGraduacaoVagasTCG($inst){
		$this->vagas = array();
		$sql = "SELECT nu_ano,nu_vagas FROM tb_reuni_vagas_tcg
				WHERE co_instituicao = $inst order by nu_ano";
		if ($this->dao->fetch($sql)) {
			while ($c = $this->dao->getRow()) {
				$this->vagasTCG[]=$c;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function AtualizarGraduacaoVagasTCG($inst,$ano,$vagas) {
		$sql = "UPDATE tb_reuni_vagas_tcg
				SET nu_vagas = ".$vagas."
				WHERE co_instituicao=".$inst." AND
			    nu_ano=".$ano;
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

    public function getGraduacaoVagasTCG() {
        return $this->vagasTCG;
    }

    public function getGraduacaoVagas() {
        return $this->vagas;
    }

    public function getCursosGrad() {
        return $this->cursos;
    }

    public function getUnidades() {
        return $this->unidades;
    }

    public function getAreas() {
        return $this->areas;
    }

}

?>
