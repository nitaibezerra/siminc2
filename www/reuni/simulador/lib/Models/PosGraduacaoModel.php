<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PosGraduacaoModel extends Model{
	private $areas;
	private $cursos;
	private $matriculados;
	private $unidades;

	public  $action;
	public  $curso;

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

	public function ListarCursosPos(){
		$this->cursos = array();
		$sql = "SELECT
				   a.co_curso,
				   b.no_unidade,
				   a.co_capes,
				   a.no_curso,
				   a.tp_modalidade,
				   a.dt_ano_inicio,
				   a.co_area,
				   a.co_conceito
				FROM
				   tb_reuni_pos_graduacao a
				   JOIN tb_reuni_unidade b on b.co_unidade = a.co_unidade
				   JOIN tb_reuni_instituicao c on c.co_instituicao = b.co_instituicao
				WHERE
				   c.co_instituicao = $this->instituicao
				ORDER BY
				   b.no_unidade,
				   a.tp_modalidade,
				   a.no_curso";
		if ($this->dao->fetch($sql)) {
			while ($curso = $this->dao->getRow()) {
				$this->cursos[]=$curso;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function InserirCursoPos($unidade, $nomeCurso, $modalidade, $inicio, $area){
		switch($modalidade) {
			case 'M': $conceito = 3; break;
			case 'D': $conceito = 4; break;
			default : $conceito = 0;
		}
		$sql = "START TRANSACTION";
		$this->dao->fetch($sql);
		$sql = "INSERT INTO tb_reuni_pos_graduacao (co_unidade, no_curso, tp_modalidade, dt_ano_inicio, co_area, co_conceito)
				VALUES ($unidade, '$nomeCurso', '$modalidade', $inicio, '$area', $conceito) RETURNING co_curso";
		$R1 = $this->dao->fetch($sql);
		$RR = $this->dao->getRow();
		$sql = "INSERT INTO tb_reuni_matricula_pos_graduacao VALUES (".$RR['co_curso'].",2006,0),(".$RR['co_curso'].",2007,0),(".$RR['co_curso'].",2008,0),(".$RR['co_curso'].",2009,0),(".$RR['co_curso'].",2010,0),(".$RR['co_curso'].",2011,0),(".$RR['co_curso'].",2012,0),(".$RR['co_curso'].",2017,0)";
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

	public function AlterarCursoPos($codCurso, $unidade, $nomeCurso, $inicio, $area){
		$sql = "START TRANSACTION";
		$this->dao->fetch($sql);
		$sql = "UPDATE tb_reuni_pos_graduacao SET
				    CO_UNIDADE    = $unidade,
				    NO_CURSO      = '$nomeCurso',
				    DT_ANO_INICIO = $inicio,
				    CO_AREA       = '$area'
				WHERE
					CO_CURSO = $codCurso";
		$R1 = $this->dao->fetch($sql);
		$sql = "UPDATE tb_reuni_matricula_pos_graduacao
				SET NU_MATRICULADOS = 0
				WHERE CO_CURSO=".$codCurso." AND
			    NU_ANO<".$inicio;
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

	public function RemoverCursoPos($curso){
		$sql = "DELETE FROM tb_reuni_pos_graduacao WHERE co_curso = $curso";
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function PegarCursoPos($codCurso, $co_curso, $co_unidade, $co_capes, $no_curso, $tp_modalidade, $dt_ano_inicio, $co_area, $co_conceito) {
		$sql = "SELECT
				   co_curso,
				   co_unidade,
				   co_capes,
				   no_curso,
				   tp_modalidade,
				   dt_ano_inicio,
				   co_area,
				   co_conceito
				FROM
				   tb_reuni_pos_graduacao
				WHERE
				   co_curso = $codCurso";
		if ($this->dao->fetch($sql)) {
			$cur = $this->dao->getRow();
			$co_curso      = $cur["co_curso"];
			$co_unidade    = $cur["co_unidade"];
			$co_capes      = $cur["co_capes"];
			$no_curso      = $cur["no_curso"];
			$tp_modalidade = $cur["tp_modalidade"];
			$dt_ano_inicio = $cur["dt_ano_inicio"];
			$co_area       = $cur["co_area"];
			$co_conceito   = $cur["co_conceito"];
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function PegarUnidadeCursoPos($codUnidade, $unidade, $municipio, $uf) {
		$m = new UnidadesModel($this->dao);
		$m->PegarUnidade($codUnidade,&$unidade,&$uf,&$comunicipio,&$municipio);
		$m = null;
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

	public function AtualizarPosGraduacaoMatriculados($codCurso,$ano,$valor) {
		$sql = "UPDATE tb_reuni_matricula_pos_graduacao
				SET nu_matriculados = ".$valor."
				WHERE co_curso=".$codCurso." AND
			    nu_ano=".$ano;
		if ($this->dao->fetch($sql)) {
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

	public function ListarPosGraduacaoMatriculados($codCurso){
		$this->matriculados = array();
		$sql = "SELECT nu_ano,nu_matriculados FROM tb_reuni_matricula_pos_graduacao
				WHERE co_curso= $codCurso order by nu_ano";
		if ($this->dao->fetch($sql)) {
			while ($c = $this->dao->getRow()) {
				$this->matriculados[]=$c;
			}
			return TRUE;
		} else {
			return $this->dao->getError();
		}
	}

    public function getPosGraduacaoMatriculados() {
        return $this->matriculados;
    }

    public function getCursosPos() {
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
