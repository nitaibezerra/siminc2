<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PlanilhasModel extends Model{
	private $matrizGlob;
	private $matrizOrc;
	public  $razaoDE;
	public  $fatProfAluno;

	private $modelInvest;
	private $totCredInvest;
	//public $modelCusteio;

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

	public function PegarRazaoDE() {
		$sql = "SELECT vl_razao_de FROM tb_reuni_parametro";

		$this->dao->fetch($sql);
		$r = $this->dao->getRow();

		return $r["vl_razao_de"];
	}

	public function PegarFatorProfAluno() {
		$sql = "SELECT vl_rel_prof_aluno FROM tb_reuni_parametro";

		$this->dao->fetch($sql);
		$r = $this->dao->getRow();

		return $r["vl_rel_prof_aluno"];
	}

	public function PegarInstituicao($cod, &$sg_inst, &$no_inst) {
		$sql = "SELECT
				   sg_instituicao,
				   no_instituicao
				FROM
				   tb_reuni_instituicao
				WHERE
				   co_instituicao = $cod
				";

		$this->dao->fetch($sql);
		$inst = $this->dao->getRow();

		$sg_inst = $inst["sg_instituicao"];
		$no_inst = $inst["no_instituicao"];
	}

	public function indicadoresGrad($tipo) {
		if($tipo == "TOTAL")
			$and = null;
		elseif($tipo == "NOTURNO")
			$and = " and b.co_turno = 'N' ";

		$sql = "				select
				   nu_ano,
				   sum(nu_cursos) as nu_cursos,
				   sum(nu_vagas) as nu_vagas,
				   sum(nu_concluintes) as nu_concluintes,
				   sum(vl_matr_proj) as vl_matr_proj
				from
				((select
				   nu_ano,
				   count(*) as nu_cursos,
				   sum(a.nu_vagas) as nu_vagas,
				   sum(a.nu_concluintes) as nu_concluintes,
				   sum(a.nu_vagas * b.vl_duracao * (1 + e.vl_fator_retencao)) as vl_matr_proj
				from
				   tb_reuni_vagas_graduacao a
				   join tb_reuni_graduacao b on b.co_curso = a.co_curso
				   join tb_reuni_unidade c on c.co_unidade = b.co_unidade
				   join tb_reuni_instituicao d on d.co_instituicao = c.co_instituicao
				   join tb_reuni_area e on e.co_area = b.co_area
				where
				   (a.nu_vagas > 0 or a.nu_concluintes > 0) and
				   d.co_instituicao = $this->instituicao
				   ".$and."
				group by
				   nu_ano
				order by
				   nu_ano)

				union

				(select
				   nu_ano,
				   0 as nu_cursos,
				   sum(a.nu_vagas) as nu_vagas,
				   sum(a.nu_concluintes) as nu_concluintes,
				   sum(a.nu_vagas * b.vl_duracao * (1 + e.vl_fator_retencao)) as vl_matr_proj
				from
				   tb_reuni_vagas_graduacao a
				   join tb_reuni_graduacao b on b.co_curso = a.co_curso
				   join tb_reuni_unidade c on c.co_unidade = b.co_unidade
				   join tb_reuni_instituicao d on d.co_instituicao = c.co_instituicao
				   join tb_reuni_area e on e.co_area = b.co_area
				where
				   not (a.nu_vagas > 0 or a.nu_concluintes > 0) and
				   d.co_instituicao = $this->instituicao
				   ".$and."
				group by
				   nu_ano
				order by
				   nu_ano)) as  S
				group by
				   nu_ano
				order by
				   nu_ano";

				//echo "<pre>".$sql."</pre>";
		$this->dao->fetch($sql);

		if($tipo == "TOTAL") {
			while ($indic = $this->dao->getRow()) {
				if($indic["nu_ano"] != 2006){
					$ncgt[$indic["nu_ano"]] = $indic["nu_cursos"];
					$nvgt[$indic["nu_ano"]] = $indic["nu_vagas"];
					$mpgt[$indic["nu_ano"]] = $indic["vl_matr_proj"];
					$adgt[$indic["nu_ano"]] = $indic["nu_concluintes"];
				}
			}
			$this->matrizGlob["ncgt"] = $ncgt;
			$this->matrizGlob["nvgt"] = $nvgt;
			$this->matrizGlob["mpgt"] = $mpgt;
			$this->matrizGlob["adgt"] = $adgt;
		}
		elseif($tipo == "NOTURNO") {
			while ($indic = $this->dao->getRow()) {
				if($indic["nu_ano"] != 2006){
					$ncgn[$indic["nu_ano"]] = $indic["nu_cursos"];
					$nvgn[$indic["nu_ano"]] = $indic["nu_vagas"];
					$mpgn[$indic["nu_ano"]] = $indic["vl_matr_proj"];
					$adgn[$indic["nu_ano"]] = $indic["nu_concluintes"];
				}
			}
			$this->matrizGlob["ncgn"] = $ncgn;
			$this->matrizGlob["nvgn"] = $nvgn;
			$this->matrizGlob["mpgn"] = $mpgn;
			$this->matrizGlob["adgn"] = $adgn;
		}

		$sql = "select
				   nu_ano+5 as nu_ano,
				   nu_vagas
				from
				   tb_reuni_vagas_tcg
				where
				   co_instituicao = $this->instituicao
				union
				select
				   a.nu_ano+5 as nu_ano,
				   sum(a.nu_vagas)
				from
				   tb_reuni_vagas_graduacao a
				   join tb_reuni_graduacao b on b.co_curso = a.co_curso
				   join tb_reuni_unidade c on c.co_unidade = b.co_unidade
				where
				   c.co_instituicao = $this->instituicao
				   and a.nu_ano in (2006, 2007, 2012)
				group by
				   a.nu_ano
				order by nu_ano
				";
		$this->dao->fetch($sql);

		while ($indic = $this->dao->getRow()) {
			$ing5[$indic["nu_ano"]] = $indic["nu_vagas"];
		}
		if (!empty($this->matrizGlob["adgt"])) {
		foreach($this->matrizGlob["adgt"] as $ind => $vlr) {
			if($ing5[$ind] > 0)
				$ntcg[$ind] = $vlr / $ing5[$ind];
			else
				$ntcg[$ind] = 0;
		}
		}
		$this->matrizGlob["ntcg"] = $ntcg;
	}

	public function indicadoresPosGrad($tipo) {
		$sql = "select
				   nu_ano,
				   sum(nu_cursos) as nu_cursos,
				   sum(nu_matriculados) as nu_matriculados
				from
				((select
				   a.nu_ano,
				   count(*) as nu_cursos,
				   sum(a.nu_matriculados) as nu_matriculados
				from
				   tb_reuni_matricula_pos_graduacao a
				   join tb_reuni_pos_graduacao b on b.co_curso = a.co_curso
				   join tb_reuni_unidade c on c.co_unidade = b.co_unidade
				   join tb_reuni_instituicao d on d.co_instituicao = c.co_instituicao
				   join tb_reuni_area e on e.co_area = b.co_area
				where
				   (a.nu_matriculados > 0) and
				   d.co_instituicao = $this->instituicao
				   and b.tp_modalidade = '$tipo'
				group by
				   nu_ano
				order by
				   nu_ano)

				union

				(select
				   a.nu_ano,
				   0 as nu_cursos,
				   sum(a.nu_matriculados) as nu_matriculados
				from
				   tb_reuni_matricula_pos_graduacao a
				   join tb_reuni_pos_graduacao b on b.co_curso = a.co_curso
				   join tb_reuni_unidade c on c.co_unidade = b.co_unidade
				   join tb_reuni_instituicao d on d.co_instituicao = c.co_instituicao
				   join tb_reuni_area e on e.co_area = b.co_area
				where
				   (a.nu_matriculados <= 0) and
				   d.co_instituicao = $this->instituicao
				   and b.tp_modalidade = '$tipo'
				group by
				   nu_ano
				order by
				   nu_ano)) as S
				group by
				   nu_ano
				order by
				   nu_ano
				";

				//echo "<pre>".$sql."</pre>";
		$this->dao->fetch($sql);
		if($tipo == "M") {
			while ($indic = $this->dao->getRow()) {
				if($indic["nu_ano"] != 2006){
					$ncpm[$indic["nu_ano"]] = $indic["nu_cursos"];
					$nvpm[$indic["nu_ano"]] = $indic["nu_matriculados"];
				}
			}
			$this->matrizGlob["ncpm"] = $ncpm;
			$this->matrizGlob["nvpm"] = $nvpm;
		}
		elseif($tipo == "D") {
			while ($indic = $this->dao->getRow()) {
				if($indic["nu_ano"] != 2006){
					$ncpd[$indic["nu_ano"]] = $indic["nu_cursos"];
					$nvpd[$indic["nu_ano"]] = $indic["nu_matriculados"];
				}
			}
			$this->matrizGlob["ncpd"] = $ncpd;
			$this->matrizGlob["nvpd"] = $nvpd;
		}
	}

	public function profEquiv() {
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
				//echo "<pre>".$sql."</pre>";
		$this->dao->fetch($sql);
		$indic = $this->dao->getRow();

		$razao = $this->PegarRazaoDE();

		foreach($indic as $ind => $valor){
			$ano = explode("_", $ind);
			$npeq[$ano[2]] = $valor;
			$ndde[$ano[2]] = $valor / $razao;
		}
		$this->matrizGlob["npeq"] = $npeq;
		$this->matrizGlob["ndde"] = $ndde;
	}

	public function alunosConceito() {
		$sql = "select
				   nu_ano,
				   sum(vl_fator * matriculados) as fav
				from (
					select
					   m.nu_ano,
					   d.vl_fator,
					   sum(m.nu_matriculados) as matriculados
					from
					   tb_reuni_matricula_pos_graduacao m
					   join tb_reuni_pos_graduacao a on a.co_curso = m.co_curso
					   join tb_reuni_unidade b on b.co_unidade = a.co_unidade
					   join tb_reuni_instituicao c on c.co_instituicao = b.co_instituicao
					   join tb_reuni_capes d on d.co_conceito = a.co_conceito
					where
					   c.co_instituicao = $this->instituicao
					group by
					   m.nu_ano,
					   d.vl_fator
					order by
					   m.nu_ano,
					   d.vl_fator
					) as tab
				group by
				   nu_ano
				order by
				   nu_ano
				";
				//echo "<pre>".$sql."</pre>";
		$this->dao->fetch($sql);
		while ($indic = $this->dao->getRow()) {
			$alFav[$indic["nu_ano"]] = $indic["fav"];
		}
		return $alFav;
	}

	public function montaMatrizOrc() {
	/***** INVESTIMENTO *****/
		$inv = $this->getInvest();

		$tpri = array(2008 => 0,
					  2009 => 0,
					  2010 => 0,
					  2011 => 0,
					  2012 => 0,
					  "Total" => 0);

		foreach($inv as $invest) {
			if($invest["co_orcamento"] == "1001") {
				$total = $invest["vl_total_2008"] + $invest["vl_total_2009"] + $invest["vl_total_2010"] + $invest["vl_total_2011"];
				$edif = array(2008 => $invest["vl_total_2008"],
							  2009 => $invest["vl_total_2009"],
							  2010 => $invest["vl_total_2010"],
							  2011 => $invest["vl_total_2011"],
							  2012 => 0,
							  "Total" => $total);
			}
			elseif($invest["co_orcamento"] == "1002") {
				$total = $invest["vl_total_2008"] + $invest["vl_total_2009"] + $invest["vl_total_2010"] + $invest["vl_total_2011"];
				$infe = array(2008 => $invest["vl_total_2008"],
							  2009 => $invest["vl_total_2009"],
							  2010 => $invest["vl_total_2010"],
							  2011 => $invest["vl_total_2011"],
							  2012 => 0,
							  "Total" => $total);
			}
			elseif($invest["co_orcamento"] == "1003") {
				$total = $invest["vl_total_2008"] + $invest["vl_total_2009"] + $invest["vl_total_2010"] + $invest["vl_total_2011"];
				$eqip = array(2008 => $invest["vl_total_2008"],
							  2009 => $invest["vl_total_2009"],
							  2010 => $invest["vl_total_2010"],
							  2011 => $invest["vl_total_2011"],
							  2012 => 0,
							  "Total" => $total);
			}

			$tpri[2008] += $invest["vl_total_2008"];
			$tpri[2009] += $invest["vl_total_2009"];
			$tpri[2010] += $invest["vl_total_2010"];
			$tpri[2011] += $invest["vl_total_2011"];
			$tpri["Total"] = $edif["Total"] + $infe["Total"] + $eqip["Total"];
		}

		$tcri = array(2008 => 0,
					  2009 => 0,
					  2010 => 0,
					  2011 => 0,
					  2012 => 0,
					  "Total" => 0);
		$cred = $this->getTotalCredInvest();
		if(!empty($cred)) {
			$total = $cred["vl_total_2008"] + $cred["vl_total_2009"] + $cred["vl_total_2010"] + $cred["vl_total_2011"];
			$tcri = array(2008 => $cred["vl_total_2008"],
						  2009 => $cred["vl_total_2009"],
						  2010 => $cred["vl_total_2010"],
						  2011 => $cred["vl_total_2011"],
						  2012 => 0,
						  "Total" => $total);
		}

		foreach($tpri as $ind => $vlr) {
			$difi[$ind] = $tcri[$ind] - $tpri[$ind];
		}

		$this->matrizOrc["edif"] = $edif;
		$this->matrizOrc["infe"] = $infe;
		$this->matrizOrc["eqip"] = $eqip;
		$this->matrizOrc["tpri"] = $tpri;
		$this->matrizOrc["tcri"] = $tcri;
		$this->matrizOrc["difi"] = $difi;
	/***** FIM INVESTIMENTO *****/

	/***** CUSTEIO *****/
		$cus = $this->getCusteio();

		$tpes = array(2008 => 0,
					  2009 => 0,
					  2010 => 0,
					  2011 => 0,
					  2012 => 0,
					  "Total" => 0);
		$tbol = array(2008 => 0,
					  2009 => 0,
					  2010 => 0,
					  2011 => 0,
					  2012 => 0,
					  "Total" => 0);

		if(!empty($cus)) {
			foreach($cus as $custeio) {
				$somaPessoal = FALSE;
				$somaBolsas = FALSE;

				if($custeio["co_orcamento"] == 6) {
					$somaPessoal = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tpeq = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 8) {
					$somaPessoal = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tsns = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 7) {
					$somaPessoal = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tsni = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 1) {
					$somaBolsas = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tbae = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 2) {
					$somaBolsas = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tbme = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 3) {
					$somaBolsas = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tbdo = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 4) {
					$somaBolsas = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tbpd = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 5) {
					$somaBolsas = TRUE;
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tbpv = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}
				elseif($custeio["co_orcamento"] == 9) {
					$total = $custeio["vl_total_2008"] + $custeio["vl_total_2009"] + $custeio["vl_total_2010"] + $custeio["vl_total_2011"] + $custeio["vl_total_2012"];
					$tubc = array(2008 => $custeio["vl_total_2008"],
								  2009 => $custeio["vl_total_2009"],
								  2010 => $custeio["vl_total_2010"],
								  2011 => $custeio["vl_total_2011"],
								  2012 => $custeio["vl_total_2012"],
								  "Total" => $total);
				}

				if($somaPessoal == TRUE) {
					$tpes[2008] += $custeio["vl_total_2008"];
					$tpes[2009] += $custeio["vl_total_2009"];
					$tpes[2010] += $custeio["vl_total_2010"];
					$tpes[2011] += $custeio["vl_total_2011"];
					$tpes[2012] += $custeio["vl_total_2012"];
					$tpes["Total"] = $tpeq["Total"] + $tsns["Total"] + $tsni["Total"];
				}

				if($somaBolsas == TRUE) {
					$tbol[2008] += $custeio["vl_total_2008"];
					$tbol[2009] += $custeio["vl_total_2009"];
					$tbol[2010] += $custeio["vl_total_2010"];
					$tbol[2011] += $custeio["vl_total_2011"];
					$tbol[2012] += $custeio["vl_total_2012"];
					$tbol["Total"] = $tbae["Total"] + $tbme["Total"] + $tbdo["Total"] + $tbpd["Total"] + $tbpv["Total"];
				}
			}
		}

		foreach($tpes as $ind => $vlr){
			$tprc[$ind] = $tpes[$ind] + $tbol[$ind] + $tubc[$ind];
		}

		$tcrc = array(2008 => 0,
					  2009 => 0,
					  2010 => 0,
					  2011 => 0,
					  2012 => 0,
					  "Total" => 0);
		$cred = $this->getTotalCredCusteio();
		if(!empty($cred)) {
			$total = $cred["vl_total_2008"] + $cred["vl_total_2009"] + $cred["vl_total_2010"] + $cred["vl_total_2011"] + $cred["vl_total_2012"];
			$tcrc = array(2008 => $cred["vl_total_2008"],
						  2009 => $cred["vl_total_2009"],
						  2010 => $cred["vl_total_2010"],
						  2011 => $cred["vl_total_2011"],
						  2012 => $cred["vl_total_2012"],
						  "Total" => $total);
		}

		foreach($tprc as $ind => $vlr) {
			$difc[$ind] = $tcrc[$ind] - $tprc[$ind];
		}

		$this->matrizOrc["tpeq"] = $tpeq;
		$this->matrizOrc["tsns"] = $tsns;
		$this->matrizOrc["tsni"] = $tsni;
		$this->matrizOrc["tpes"] = $tpes;

		$this->matrizOrc["tbae"] = $tbae;
		$this->matrizOrc["tbme"] = $tbme;
		$this->matrizOrc["tbdo"] = $tbdo;
		$this->matrizOrc["tbpd"] = $tbpd;
		$this->matrizOrc["tbpv"] = $tbpv;
		$this->matrizOrc["tbol"] = $tbol;

		$this->matrizOrc["tubc"] = $tubc;

		$this->matrizOrc["tprc"] = $tprc;
		$this->matrizOrc["tcrc"] = $tcrc;
		$this->matrizOrc["difc"] = $difc;
	/***** FIM CUSTEIO *****/
	}

	public function montaMatrizGlob() {
    	$this->indicadoresGrad("TOTAL");
    	$this->indicadoresGrad("NOTURNO");
    	$this->indicadoresPosGrad("M");
    	$this->indicadoresPosGrad("D");
    	$this->profEquiv();

    	//C�lculo da m�dia de alunos matriculados por professor com equival�ncia DE
    	foreach($this->matrizGlob as $ind => $valor) {
    		switch($ind) {
    			case "nvpm": $nvpm = $valor; break;
    			case "nvpd": $nvpd = $valor; break;
    			case "mpgt": $mpgt = $valor; break;
    			case "ndde": $ndde = $valor; break;
    		}
    	}

		if(!empty($nvpm)) {
			foreach($nvpm as $ind => $vlr) {
				$med[$ind] = $vlr;
			}
		}

		if(!empty($nvpd)) {
			foreach($nvpd as $ind => $vlr) {
				$med[$ind]+= $vlr;
			}
		}

		if(!empty($ndde)) {
			foreach($ndde as $ind => $vlr) {
				if(empty($vlr))
					$med[$ind] = 0;
				else
					$med[$ind]/= $vlr;
			}
		}
   		//Fim do calc da média

    	//C�lculo da multiplicaçõo de alunos por conceito
    	$alFav = $this->alunosConceito();
		$fat = $this->PegarFatorProfAluno();

		if(!empty($alFav)) {
	    	foreach($alFav as $ind => $vlr) {
				if($ind != 2006){
	    			$ndpg[$ind] = $vlr;
	    		}
			}
		}
   		//Fim do calc da multiplicação
    	//Cálculo da dedução da pós-grad
    	foreach($ndde as $ind => $vlr) {
    		$media = $med[$ind];
    		$dde = $ndde[$ind]*0.05;

			if($ind != 2006){
	    		if($med[$ind] > 1.5) {
	    			$dpg = ($ndpg[$ind] - (1.5*$vlr)) / $fat;
	    			if($dpg > $dde)
	    				$ndpg[$ind] = $dpg;
	    			else
	    				$ndpg[$ind] = $dde;
	    		}
	    		else {
	    			$dpg = $ndpg[$ind] / $fat;
	    			if($dpg < $dde)
	    				$ndpg[$ind] = $dpg;
	    			else
	    				$ndpg[$ind] = $dde;
	    		}
			}
    	}
    	$this->matrizGlob["ndpg"] = $ndpg;
    	//Fim do cálculo da dedução da pós-grad

    	//Cálculo do corpo docente ajustado
		if(!empty($ndde)) {
	    	foreach($ndde as $ind => $vlr) {
	    		$cdaj[$ind] = $vlr - $ndpg[$ind];
	    	}
	    	$this->matrizGlob["cdaj"] = $cdaj;
		}
    	//Fim do cálculo do corpo docente ajustado

    	//Cálculo da relação aluno por professor
		if(!empty($mpgt)) {
	    	foreach($mpgt as $ind => $vlr) {
				if(empty($cdaj[$ind]))
		    		$ragp[$ind] = 0;
				else
		    		$ragp[$ind] = $vlr / $cdaj[$ind];
	    	}
    		$this->matrizGlob["ragp"] = $ragp;
		}
    	//Fim do cálculo da relação aluno por professor
	}

	public function getMatrizGlob() {
		$this->montaMatrizGlob();
		return $this->matrizGlob;
	}

	public function getMatrizOrc() {
		$this->montaMatrizOrc();
		return $this->matrizOrc;
	}

	public function getInvest() {
		$m = new InvestimentoModel($this->dao);
		$m->instituicao = $this->instituicao;
    	$m->ListarInvestimento();
    	return $m->getInvestimento();
	}

	public function getTotalInvest() {
		$m = new InvestimentoModel($this->dao);
		$m->instituicao = $this->instituicao;
    	$m->ListarInvestimento();
    	return $m->getTotalInvestimento();
	}

	public function getTotalCredInvest() {
		$m = new InvestimentoModel($this->dao);
		$m->instituicao = $this->instituicao;
    	$m->ListarInvestimento();
    	return $m->getTotalCreditos();
	}

	public function getCusteio() {
		$m = new CusteioModel($this->dao);
		$m->instituicao = $this->instituicao;
    	$m->ListarCusteio();
    	return $m->getCusteio();
	}

	public function getTotalCredCusteio() {
		$m = new CusteioModel($this->dao);
		$m->instituicao = $this->instituicao;
    	$m->ListarCusteio();
    	return $m->getTotalCreditos();
	}
}

?>
