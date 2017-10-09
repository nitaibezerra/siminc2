<?php
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/library/simec/Grafico.php";

class Demanda extends Modelo
{
	// Tipos de demanda
	const K_TIPO_JUDICIAL = 1;
	const K_TIPO_EXTRA_JUDICIAL = 2;
	const K_TIPO_ADMINISTRATIVO = 3;

	// Competencias
	const K_COMPETENCIA_FEDERAL = 1;
	const K_COMPETENCIA_ESTADUAL = 2;
	const K_COMPETENCIA_TRABALHISTA = 3;

	// Status decisao
	const K_DECISAO_DEFERIDA = 'DF';
	const K_DECISAO_INDEFERIDA = 'ID';
	const K_DECISAO_PARCIALMENTE_DEFERIDA = 'PD';

	// Cumprimento
	const K_CUMPRIMENTO_DECISAO_CUMPRIDA = 'DC ';
	const K_CUMPRIMENTO_DECISAO_CUMPRIDA_PARCIALMENTE = 'DCP';
	const K_CUMPRIMENTO_DECISAO_NAO_CUMPRIDA = 'DNC';
	private $view;

	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "demandasfies.demanda";

	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("dmdid");

	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array('dmdid' => null, 'docid' => null, 'usucpfinclusao' => null, 'dmddtinclusao' => null, 'usucpfalteracao' => null, 'dmddtalteracao' => null, 'dmdstatus' => null, 'usucpfinativacao' => null, 'dmddtinativacao' => null, 'dmdtipo' => null, 'dmdprazo' => null, 'dmdprocesso' => null, 'dmdobjeto' => null, 'dmduf' => null, 'dmdtribunalorigem' => null, 'dmdcompetencia' => null, 'dmdvalorcausa' => null, 'dmdnumeroprocessoadm' => null, 'dmdcnpjmantedora' => null, 'dmdliminar' => null, 'dmdmulta' => null, 'dmdjuizadoespecial' => null, 'dmdchaveacesso' => null, 'dmdvara' => null, 'dmdcnpjies' => null, 'objid' => null, 'trbid' => null, //        'claid' => null,
		'claiddti' => null, 'dmddtentradaprofe' => null, 'dmddecisaofnde' => null, 'acjid' => null, 'decid' => null, 'solid' => null, 'dmdstatusatendimento' => null, 'dmddiasatendimento' => null, 'dmddecisaorecorrivel' => null, 'dmdmultadsc' => null, 'dmdprazotecnico' => null, 'dmdprazojudicial' => null, 'dmdtransitojulgado' => null, 'dmdfavoravel' => null, 'dmdstatusdecisao' => null, 'dmdcumprimento' => null);

	public function getComboComprimento()
	{
		$dmdcumprimento = $_POST['dmdcumprimento'];
		$dados = array(array('codigo' => 'DC', 'descricao' => 'Decisão Cumprida'), array('codigo' => 'DCP', 'descricao' => 'Decisão Cumprida Parcialmente'), array('codigo' => 'DNC', 'descricao' => 'Decisão Não Cumprida'),);
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'objid', array($dmdcumprimento));
	}

	public function getTribunais($dmdcompetencia = 0)
	{
		$dmdcompetencia = $dmdcompetencia ? $dmdcompetencia : 0;
		$and = '';
		if ($dmdcompetencia) {
			$and = " and trbcompetencia = $dmdcompetencia ";
		}
		$sql = "select trbid as codigo, trbnome as descricao
		        from demandasfies.tribunal
		        where trbstatus = 'A'
               	{$and}
		        order by descricao";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		$dados = ($dados);

		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'trbid');
	}

	public function getClassificacao($clatipo = 'G')
	{
		$dmdid = $this->dmdid ? $this->dmdid : 0;

		$sql = "select claid as codigo, clanome as descricao from demandasfies.classificacao
		        where clastatus = 'A'
		        and clatipo = '{$clatipo}'
		        order by descricao";

		$dados = $this->carregar($sql);
		$dados = ($dados);

		$dados = $dados ? $dados : array();

		$campo = $clatipo == 'T' ? 'claiddti' : null;

		if ($dmdid) {
			$sql = "select cl.claid as codigo
                from demandasfies.demandaclassificacao dcl
                inner join demandasfies.classificacao cl on cl.claid = dcl.claid
		        where dmcstatus = 'A'
                        and cl.clatipo = '{$clatipo}'
		        and dmdid = {$dmdid}
		        ";

			$valores = $this->carregar($sql);
			$valores = $valores ? $valores : array();

			$aValores = array();
			if ($clatipo == 'G' || $clatipo == 'D') {
				foreach ($valores as $valor) {
					$aValores[] = $valor['codigo'];
				}
			}
		}
		if (!empty($_POST['claid']) && $clatipo == 'G') {
			$aValores = $_POST['claid'];
		}
		return $this->getOptions($dados, array('prompt' => ' Selecione '), $campo, $aValores);
	}

	public function getDecisao()
	{

		$sql = "select decid as codigo, decnome as descricao from demandasfies.decisao
		        where decstatus = 'A'
		        order by descricao";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		$dados = ($dados);

		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'decid');
	}

	public function getSolicitacao()
	{

		$sql = "select solid as codigo, solnome as descricao from demandasfies.solicitacao
		        where solstatus = 'A'
		        order by descricao";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'solid');
	}

	public function getMaterias()
	{
		$dmdid = $this->dmdid ? $this->dmdid : 0;

		$sql = "select matid as codigo, matnome as descricao from demandasfies.materia
		        where matstatus = 'A'
		        order by descricao";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();

		$sql = "select matid as codigo
                from demandasfies.demandamateria
		        where dmmstatus = 'A'
		        and dmdid = {$dmdid}
		        ";

		$valores = $this->carregar($sql);
		$valores = $valores ? $valores : array();

		$aValores = array();
		foreach ($valores as $valor) {
			$aValores[] = $valor['codigo'];
		}

		return $this->getOptions($dados, array('prompt' => ' Selecione '), null, $aValores);
	}

	public function getTipoAcao()
	{

		$sql = "select acjid as codigo, acjnome as descricao from demandasfies.acaojudicial
		        where acjstatus = 'A'
		        order by descricao";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		$dados = ($dados);
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'acjid');
	}

	public function getTipoAcaoDescricao($acjid)
	{
		$sql = "SELECT acjnome FROM demandasfies.acaojudicial WHERE acjstatus = 'A' AND acjid = {$acjid} ";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		if ($dados) {
			return $dados[0]['acjnome'];
		}
		return null;
	}

	public function getComboUfs()
	{
		$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'dmduf');
	}

	public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $aValores = array())
	{
		$html = '';
		$selected = '';
		$aValores = is_array($aValores) ? $aValores : (array)$aValores;

		if (isset ($htmlOptions ['prompt'])) {
			$html .= '<option value="">' . strtr($htmlOptions ['prompt'], array('<' => '&lt;', '>' => '&gt;')) . "</option>\n";
		}

		if ($dados) {
			foreach ($dados as $data) {
				$selected = in_array($data['codigo'], $aValores) ? "selected='true' " : "";
				if ($idCampo && $this->$idCampo) {
					$selected = ($data['codigo'] == $this->$idCampo ? "selected='true' " : "");
				}

				$html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data['codigo'] . ">  " . simec_htmlentities($data['descricao']) . " </option> ";
			}
		}
		return $html;
	}

	public function getAttributeValue($id)
	{
		return $this->arAtributos [$id];
	}

	public function getCompetencias()
	{
		$dados = array(array('codigo' => self::K_COMPETENCIA_FEDERAL, 'descricao' => 'Justiça Federal'), array('codigo' => self::K_COMPETENCIA_ESTADUAL, 'descricao' => 'Justiça Estadual'), array('codigo' => self::K_COMPETENCIA_TRABALHISTA, 'descricao' => 'Justiça Trabalhista'));
		$dados = ($dados);
		return $this->getOptions($dados, array(), 'dmdcompetencia');
	}

	public static function getDescricaoTipo($cod)
	{
		$dados = array(self::K_TIPO_JUDICIAL, 'Judicial', self::K_TIPO_EXTRA_JUDICIAL => 'Extra-Judicial', self::K_TIPO_ADMINISTRATIVO => 'Administrativo');
		return $dados[$cod];
	}

	public function getTipos()
	{
		$dados = array(array('codigo' => self::K_TIPO_JUDICIAL, 'descricao' => 'Judicial'), array('codigo' => self::K_TIPO_EXTRA_JUDICIAL, 'descricao' => 'Extra-Judicial'), array('codigo' => self::K_TIPO_ADMINISTRATIVO, 'descricao' => 'Administrativo'));
		return $this->getOptions($dados, array('prompt' => 'Selecione'), 'dmdtipo');
	}

	public function getStatusDecisao()
	{
		$dados = array(array('codigo' => self::K_DECISAO_DEFERIDA, 'descricao' => 'Deferida'), array('codigo' => self::K_DECISAO_INDEFERIDA, 'descricao' => 'Indeferida'), array('codigo' => self::K_DECISAO_PARCIALMENTE_DEFERIDA, 'descricao' => 'Parcialmente Deferida'));
		return $this->getOptions($dados, array(), 'dmdtipo');
	}

	public static function getDescricaoStatusDecisao($dmdstatusdecisao)
	{
		$dados = array(self::K_DECISAO_DEFERIDA => 'Deferida', self::K_DECISAO_INDEFERIDA => 'Indeferida', self::K_DECISAO_PARCIALMENTE_DEFERIDA => 'Parcialmente Deferida');
		return $dados[$dmdstatusdecisao];
	}

	public function listarDemandasPorPerfil($perfis)
	{
		$where = '';
		$tipodemanda = $_POST['tipodemanda'];
		if (!empty($tipodemanda)) {
			$where = " AND aed.esdidorigem in({$tipodemanda})";
		}
		if (in_array(PFL_DISTRIBUIDOR, $perfis)) {
			$perfis[] = PFL_NUCLEO_JURIDICO;
		}

		if (!empty ($perfis)) {
			$sql = $this->getSqlListagem();
			$sql .= "   INNER JOIN workflow.acaoestadodoc ae on ae.esdidorigem = ed.esdid
						INNER JOIN workflow.acaoestadodoc aed on aed.aedid = hd.aedid
                        INNER JOIN workflow.estadodocumentoperfil ep on ep.aedid = ae.aedid
                     WHERE ep.pflcod in ( " . implode(', ', $perfis) . " )
                     AND d.dmdstatus = 'A'
                     {$where}
                     AND (
                         (ed.esdid = 1271) OR -- Procuradoria Federal
                         (ed.esdid = 1271) OR -- Procuradoria Federal
                         (ed.esdid = 1270) OR -- Nucleo Juridico
                         (ed.esdid = 1272 and rea.usucpf = '{$_SESSION['usucpf']}') or -- Advogado
                         (ed.esdid = 1282) OR -- DTI/MEC
                         (ed.esdid = 1273 and rea.usucpf = '{$_SESSION['usucpf']}') OR -- Analista DTI/MEC
                         (ed.esdid = 1283 and rea.usucpf = '{$_SESSION['usucpf']}') OR -- Execucao DTI/MEC
                         (ed.esdid = 1274) OR -- 4 Nivel
                         (ed.esdid = 1284) OR  -- Gestor FIES
                         (ed.esdid = 1374)  -- Gerencia DIGEF
                     )
                     ORDER BY rea.reaprazo ASC, d.acjid, d.dmdprazo
                     ";
//ver($sql, d);
			$dados = $this->carregar($sql);
			$dados = $dados ? $dados : array();

			echo '<fieldset><legend>Minhas demandas <span style="color: red;">(' . count($dados) . ')</span></legend>';
			$this->montarTabelaListagem($dados);
			echo '</fieldset>';
		}
	}

	public function exibirGraficoPorSituacao()
	{
		$grafico = new Grafico(Grafico::K_TIPO_PIZZA, false);

		$sql = "select count(*) valor, ed.esddsc descricao, ed.esdid
                                from demandasfies.demanda d
                                        inner join workflow.documento doc on doc.docid = d.docid
                                        inner join workflow.estadodocumento ed on ed.esdid = doc.esdid
                                group by ed.esddsc, ed.esdid";

		$dados = $this->carregar($sql);
		$grafico->setTitulo('Demandas por Situação')->gerarGrafico($dados);
	}

	public function getDadosGraficoEmDistribuicaoValidacao()
	{
		$grafico = new Grafico(Grafico::K_TIPO_PIZZA, false);
		$sql = $this->getSqlQuantidadeEmDistribuicaoValidacao();
		$dados = $this->carregar($sql);

		$grafico->setTitulo('Demandas aguardando Distribuição/Validação')
				->setAgrupadores(array('name' => 'tipo', 'valor' => 'valor'))
				->gerarGrafico($dados);
	}

	public function listarQuantidadePorSituacao()
	{
		$sql = "select ed.esdid, esddsc, valor
                        from workflow.estadodocumento ed
                                left join (
                                        select count(*) valor, doc.esdid
                                        from demandasfies.demanda d
                                                inner join workflow.documento doc on doc.docid = d.docid
                                        group by doc.esdid
                                ) q on q.esdid = ed.esdid
                        where tpdid = " . WF_TPDID_DEMANDASFIES_DEMANDA . "
                        and esdstatus = 'A'
                        order by esdordem";

		$listagem = new Simec_Listagem (Simec_Listagem::RELATORIO_CORRIDO);
		$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valor'));
		$listagem->addCallbackDeCampo(array('esddsc'), 'alinhaParaEsquerda');
		$listagem->setCabecalho(array('Situação', 'Quantidade'));
		$listagem->addAcao('view', 'filtrarSituacao');
		$listagem->setQuery($sql);
		$listagem->render();
	}

	public function montarTabelaListagem($dados = array())
	{
		?>
		<div id="listagem">
			<?php
			$arrayTituloEsconde = $this->getTituloEEscondeColuna();

			$listagem = new Simec_Listagem();
			$listagem->setTamanhoPagina(50);
			$listagem->turnOnPesquisator();
			$listagem->addCallbackDeCampo(array('objnome', 'autor', 'esddsc'), 'alinhaParaEsquerda');
			$listagem->addCallbackDeCampo(array('acjnome'), 'tratarDemandaJudicialLista');

			$listagem->addAcao('edit', 'editarDemanda');
			$listagem->addAcao('info', array('func' => 'exibirHistorico', 'extra-params' => array('docid')));
			$listagem->addAcao('print', 'imprimirResumo');
			$listagem->esconderColunas($arrayTituloEsconde['colunasEscondidas']);
			$listagem->setCabecalho($arrayTituloEsconde['tituloCabecalho']);
			$listagem->setDados($dados);
			$listagem->render();
			?>
		</div>
	<?php
	}

	public function getSqlListagem()
	{
		$perfis = pegaPerfilGeral($_SESSION['usucpf']);

		$esd_nucleo_juridico = ESD_DEMANDA_NUCLEO_JURIDICO;
		$esd_em_cadastramento = ESD_DEMANDA_EM_CADASTRAMENTO;
		$esd_profe = ESD_DEMANDA_PROFE;
		$esd_advogado = ESD_DEMANDA_ADVOGADO;
		$esd_analista_dti = ESD_DEMANDA_ANALISTA_DTI_MEC;
		$esd_dti = ESD_DEMANDA_DTI_MEC;

		$novosCampos = '';

		if (in_array(PFL_DTI_MEC, $perfis) || in_array(PFL_ANALISTA_DTI_MEC, $perfis) || in_array(PFL_ADVOGADO, $perfis)) {
			$novosCampos .= "
			 TO_CHAR( (
				SELECT MAX(htddata)
				FROM workflow.historicodocumento hst
				INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid AND aed.esdiddestino = {$esd_dti}
				WHERE hst.docid = d.docid
			 ), 'DD/MM/YYYY') AS htddata_analista_dti,
			 TO_CHAR( (
				SELECT MAX(htddata)
				FROM workflow.historicodocumento hst
				INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid AND aed.esdiddestino = {$esd_analista_dti}
				WHERE hst.docid = d.docid
			 ), 'DD/MM/YYYY') AS htddata_dti,
		   TO_CHAR(reaAnalistaDti.reaprazo, 'DD/MM/YYYY') as prazo_resp_analista_dti,
";
		}


		$sql = "
SELECT distinct d.dmdid, d.dmdid as codigo, d.docid,
	   ( SELECT array_to_string(array_agg(clanome), ', ') FROM demandasfies.demandaclassificacao dcl
			INNER JOIN demandasfies.classificacao cl on (cl.claid, d.dmdid, dmcstatus, clastatus) = ( dcl.claid, dcl.dmdid,'A','A'))  as objnome,
	   array_to_string(array(select ((coalesce((public.formata_cpf_cnpj(dpacpf)),'') || coalesce((public.formata_cpf_cnpj(dpacnpj)),'')) || ' - ' || dpanome) as autor from demandasfies.demandapartesacao dpa where dpastatus = 'A' and dpatipo = 'A' and dpa.dmdid = d.dmdid), '<br />') as autor,
	   ( select usunome from seguranca.usuario usu where usu.suscod != 'B' and  usu.usucpf =  rea.usucpf ) AS responsavel,
	   dmdprocesso,

	   CASE
		   WHEN coalesce(reaNucleoJuridico.reaprazo::char, '') = '' then ''

		   WHEN EXTRACT(DAYS FROM(reaNucleoJuridico.reaprazo - NOW()))::integer < 0
			   THEN '<span style=\"color: red;\">' || (-1)*EXTRACT(DAYS FROM(reaNucleoJuridico.reaprazo - NOW())) || ' dias </span>'

	   end as prazoarea,
	   EXTRACT(DAYS FROM(NOW() - coalesce(hd.htddata, doc.docdatainclusao)))::integer as diassituacao,
	   ed.esddsc,
	   TO_CHAR(reaAdvogado.reaprazo, 'DD/MM/YYYY') as prazo_resp_advogado,
	   TO_CHAR(rea.reaprazo, 'DD/MM/YYYY') as prazo_resp,
	    acjnome,
	   {$novosCampos}
	   d.acjid,
	   d.dmdprazo,
	   rea.reaprazo,
	   TO_CHAR(reaProfe.reaprazo, 'DD/MM/YYYY') as prazo_resp_prof,
		CASE
		WHEN EXTRACT(DAYS FROM(NOW() - reaProfe.reaprazo ))::integer > 0
		THEN EXTRACT(DAYS FROM(NOW() - reaProfe.reaprazo ))::integer  || ' dias'
		ELSE '' end as dias_em_atraso_profe

FROM demandasfies.demanda d
	INNER JOIN workflow.documento doc on doc.docid = d.docid
	INNER JOIN workflow.estadodocumento ed on ed.esdid = doc.esdid
	LEFT JOIN workflow.historicodocumento hd on hd.hstid = doc.hstid
	LEFT  JOIN demandasfies.responsavelarea rea on case when doc.esdid = {$esd_profe} then (rea.dmdid, rea.esdid, rea.reastatus) = (d.dmdid, {$esd_em_cadastramento}, 'A') else (rea.dmdid, rea.esdid, rea.reastatus) = (d.dmdid, doc.esdid, 'A'
	) end
	LEFT  JOIN demandasfies.responsavelarea reaNucleoJuridico on (reaNucleoJuridico.dmdid, reaNucleoJuridico.esdid, reaNucleoJuridico.reastatus) = (d.dmdid, {$esd_nucleo_juridico}, 'A')
	LEFT  JOIN demandasfies.responsavelarea reaProfe on  (reaProfe.dmdid, reaProfe.esdid, reaProfe.reastatus) = (d.dmdid, {$esd_em_cadastramento}, 'A' )
	LEFT  JOIN demandasfies.responsavelarea reaAdvogado on  (reaAdvogado.dmdid, reaAdvogado.esdid, reaAdvogado.reastatus) = (d.dmdid, {$esd_advogado}, 'A' )
	LEFT  JOIN demandasfies.responsavelarea reaAnalistaDti on  (reaAnalistaDti.dmdid, reaAnalistaDti.esdid, reaAnalistaDti.reastatus) = (d.dmdid, {$esd_analista_dti}, 'A' )
	LEFT  JOIN demandasfies.acaojudicial acj on acj.acjid = d.acjid ";
		return $sql;
	}

	public function recuperarListagem()
	{

		$sql = $this->getSqlListagem();

		if ($_POST ['action'] == 'pesquisar' || !empty($_REQUEST['esdid'])) {

			$this->popularDadosObjeto();
			$demandaPartesAcao = new DemandaPartesAcao ();
			$demandaPartesAcao->popularDadosObjeto();
			$where = $wherePartes = $join = array();
			$esdid_entrega = $_POST['esdid_entrega'];

			if ($esdid_entrega == 't') {
				$esdidAguardando = ESD_ENTREGA_AGUARDANDO_APROVACAO;
				$where [] = " est_entrega.esdid = {$esdidAguardando}";
				$join[] = "
							left join demandasfies.demandaentrega de on de.dmdid = d.dmdid
				 			INNER JOIN workflow.documento doc_entrega ON doc_entrega.docid = de.docid
                            INNER JOIN workflow.estadodocumento est_entrega on est_entrega.esdid = doc_entrega.esdid
				 			";
			}

			if ($this->dmdid) {
				$where [] = "d.dmdid = {$this->dmdid}";
			}
			if ($this->dmdprazo) {
				$dmdprazo = formata_data_sql($this->dmdprazo);
				$where [] = "d.dmdprazo = '{$dmdprazo}'";
			}
			if ($this->dmdprocesso) {
				$where [] = "d.dmdprocesso ilike '%{$this->dmdprocesso}%' ";
			}
//			if ($this->dmdmandatoseguranca) {
//				$where [] = "d.dmdmandatoseguranca = '{$this->dmdmandatoseguranca}' ";
//			}
			if ($this->dmdliminar) {
				$where [] = "d.dmdliminar = '{$this->dmdliminar}' ";
			}
			if ($this->dmdmulta) {
				$where [] = "d.dmdmulta = '{$this->dmdmulta}' ";
			}
			if (!empty($_REQUEST['esdid'])) {
				$where [] = " doc.esdid = {$_REQUEST['esdid']} ";
			}
			if (!empty($_REQUEST['usucpf'])) {
				$where [] = " usu.usucpf = '{$_REQUEST['usucpf']}' ";
				$join[] = " INNER JOIN seguranca.usuario usu ON usu.usucpf = rea.usucpf  ";
			}

			if (!empty($_POST['area'])) {

				switch ($_POST['area']) {
					case (PFL_PROCURADOR_FEDERAL):
						$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_EM_CADASTRAMENTO . ') ';
						break;
					case (PFL_NUCLEO_JURIDICO):
						$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_NUCLEO_JURIDICO . ', ' . ESD_DEMANDA_ADVOGADO . ') ';
						break;
					case (PFL_DTI_MEC):
						$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_DTI_MEC . ', ' . ESD_DEMANDA_EXECUCAO_DTI_MEC . ', ' . ESD_DEMANDA_ANALISTA_DTI_MEC . ') ';
						break;
					case (PFL_4_NIVEL):
						$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_4_NIVEL . ') ';
						break;
					case (PFL_GESTOR_FIES):
						$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_GESTOR_FIES . ') ';
						break;
					default:
						$pesquisa = ' doc.esdid in (' . $_POST['area'] . ') ';
				}
				$where [] = $pesquisa;
			}

			$joinPartes = false;
			if ($demandaPartesAcao->dpacpf) {
				$demandaPartesAcao->dpacpf = str_replace(array('.', '-'), '', $demandaPartesAcao->dpacpf);
				$where[] = "dpacpf = '{$demandaPartesAcao->dpacpf}' ";
				$joinPartes = true;
			}
			if ($demandaPartesAcao->dpacnpj) {
				$demandaPartesAcao->dpacnpj = str_replace(array('/', '.', '-'), '', $demandaPartesAcao->dpacnpj);
				$where[] = "dpacnpj = '{$demandaPartesAcao->dpacnpj}' ";
				$joinPartes = true;
			}
			if ($demandaPartesAcao->dpanome) {
				$dpanome = removeAcentos($demandaPartesAcao->dpanome);
				$where[] = " dpanome ilike ('%{$dpanome}%') ";
				$joinPartes = true;
			}

			if ($joinPartes) {
				$join[] = " inner join demandasfies.demandapartesacao dpa on dpa.dmdid = d.dmdid";
			}

			$join = array_filter($join);

			if (!empty ($join)) {
				$join = implode('  ', $join);
				$sql .= $join;
			}

			$sql .= " WHERE d.dmdstatus = 'A' ";

			$where = array_filter($where);
			if (!empty ($where)) {
				$where = implode(' AND ', $where);
				// $sql .= implode(' ',join);
				$sql .= ' AND ' . $where;
			}
		} else {
			$sql .= " WHERE d.dmdstatus = 'A' ";
		}
		$sql .= ' ORDER BY rea.reaprazo ASC, d.acjid, d.dmdprazo';
//ver($sql, d);
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		$this->montarTabelaListagem($dados);
	}

	public function getDemandaVencida()
	{
		function contagem()
		{
			static $i = 1;
			return str_pad($i++, 3, 0, STR_PAD_LEFT);
		}

		$esd_advogado = ESD_DEMANDA_ADVOGADO;
		$esd_em_cadastramento = ESD_DEMANDA_EM_CADASTRAMENTO;
		$esd_nucleo_juridico = ESD_DEMANDA_NUCLEO_JURIDICO;
		$sql = "
					SELECT
						DISTINCT
						1 AS cont,
						(d.dmdid),
                       array_to_string(array(select ((coalesce((public.formata_cpf_cnpj(dpacpf)),'') || coalesce((public.formata_cpf_cnpj(dpacnpj)),'')) || ' - ' || dpanome) as autor from demandasfies.demandapartesacao dpa where dpastatus = 'A' and dpatipo = 'A' and dpa.dmdid = d.dmdid), '<br />') as autor,
                       dmdprocesso,
                       ( select usunome from seguranca.usuario usu where usu.suscod != 'B' and  usu.usucpf =  rea.usucpf ) AS responsavel,
                     	acjnome,

                       case
                           when coalesce(reaNucleoJuridico.reaprazo::char, '') = '' then ''
                           when EXTRACT(DAYS FROM(reaNucleoJuridico.reaprazo - NOW()))::integer < 0
                               then (-1) * EXTRACT(DAYS FROM(reaNucleoJuridico.reaprazo - NOW())) || '  '
                       end as prazoarea,
                       CASE
                           WHEN ed.esddsc = 'Advogados'
                               THEN  (
								  SELECT usunome FROM demandasfies.responsavelarea reaAdvogado
									INNER JOIN seguranca.usuario usu ON usu.usucpf = reaAdvogado.usucpf
									WHERE usu.suscod != 'B'
									AND reaAdvogado.dmdid = d.dmdid
									AND reaAdvogado.esdid = {$esd_advogado}
									AND reaAdvogado.reastatus = 'A'
                               )
							   ELSE  ed.esddsc
                       END AS esddsc,
                       TO_CHAR(reaNucleoJuridico.reaprazo, 'DD/MM/YYYY') as prazo_resp,
					   CASE
                           WHEN d.dmdstatusatendimento = 'A'
                               THEN TO_CHAR( rea.reaprazo::date + d.dmddiasatendimento, 'DD/MM/YYYY' )
                       END AS data_atendimento,
                       CASE
                           WHEN d.dmdstatusatendimento = 'A' AND (rea.reaprazo::date + d.dmddiasatendimento) - CURRENT_DATE > 0
                               THEN (rea.reaprazo::date + d.dmddiasatendimento) - CURRENT_DATE
                       END AS dias_restantes,

					   reaNucleoJuridico.reaprazo,
                       d.acjid,
                       d.dmdprazo
                FROM demandasfies.demanda d
                    INNER JOIN workflow.documento doc on doc.docid = d.docid
                    INNER JOIN workflow.estadodocumento ed on ed.esdid = doc.esdid
                    LEFT JOIN workflow.historicodocumento hd on hd.hstid = doc.hstid
                    LEFT  JOIN demandasfies.responsavelarea rea on  (rea.dmdid, rea.esdid, rea.reastatus) = (d.dmdid, {$esd_em_cadastramento}, 'A' )
                    LEFT  JOIN demandasfies.responsavelarea reaNucleoJuridico on (reaNucleoJuridico.dmdid, reaNucleoJuridico.esdid, reaNucleoJuridico.reastatus) = (d.dmdid, {$esd_nucleo_juridico}, 'A')
                    LEFT  JOIN demandasfies.acaojudicial acj on acj.acjid = d.acjid
				WHERE
					rea.reaprazo IS NOT NULL
					AND ed.esdid != 1275
					AND EXTRACT( DAYS FROM(reaNucleoJuridico.reaprazo - NOW()))::integer < 0

				ORDER BY reaNucleoJuridico.reaprazo ASC, d.acjid, d.dmdprazo
		";
//ver($sql, d);
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		$tituloCabecalho = array(' ', 'Código', 'CPF/Autor da demanda judicial', 'No. Processo', 'Responsável PROFE', 'Tipo Ação', 'Dias em Atraso', 'Responsável DIGEF', 'Data Núcleo Jurídico', 'Em Atendimento até', 'Dias Restantes');
		$colunasEscondidas = array('acjid', 'reaprazo', 'dmdprazo');

		$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
		$listagem->addCallbackDeCampo(array('objnome', 'autor', 'esddsc', 'responsavel'), 'alinhaParaEsquerda');
		$listagem->addCallbackDeCampo(array('acjnome'), 'tratarDemandaJudicialLista');
		$listagem->addCallbackDeCampo(array('cont'), 'contagem');
		$listagem->esconderColunas($colunasEscondidas);
		$listagem->setCabecalho($tituloCabecalho);
		$listagem->setDados($dados);
		return $listagem->render();
	}

	public function verificarPermissaoEdicao($esdid, $perfis)
	{
		if ($this->testa_superuser() || in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_PROCURADOR_FEDERAL, $perfis) || (in_array(PFL_NUCLEO_JURIDICO, $perfis) && in_array($esdid, array(ESD_DEMANDA_NUCLEO_JURIDICO))) || (in_array(PFL_ADVOGADO, $perfis) && in_array($esdid, array(ESD_DEMANDA_ADVOGADO))) || (in_array(PFL_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_EM_CADASTRAMENTO))) || (in_array(PFL_ANALISTA_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC))) || (in_array(PFL_4_NIVEL, $perfis) && in_array($esdid, array(ESD_DEMANDA_4_NIVEL)))
		) {
			return true;
		}

		return false;
	}

	public function recuperarCamposEditaveis($perfis)
	{
	}

	public function getParansEmailAlteracaoDataArea($dataAntiga, $dataNova, $reajustificativa, $areaSelecionada)
	{
		$paransEmail['remetente'] = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");
		$paransEmail['assunto'] = "[Demandas FIES] Mudança do PRAZO da Área de {$dataAntiga} para {$dataNova} )";

		$paransEmail['mensagem'] = "<pre>Prezados,

Foi alterado o prazo para o envio da Demanda Nº <span style='color: red;'>{$this->dmdid}</span> da Área {$areaSelecionada}, de {$dataAntiga} para {$dataNova}.

Justificativa: {$reajustificativa}

Para maiores detalhes, favor entrar no SIMEC, módulo Demandas FIES em http://simec.mec.gov.br.

Atenciosamente,
Equipe SIMEC.
</pre>
        ";

		return $paransEmail;
	}

	public function getDadosRelatorioDemandasPorUsuario()
	{
		$where = '';
		$sisId = SIS_DEMANDASFIES;
		if ($_POST ['action'] == 'pesquisar') {
			$area = (int)$_POST ['area'];
			$responsavel = trim($_POST ['responsavel']);
			if (!empty($area)) {
				$where .= " AND pflcod = {$area}";
			}
			if (!empty($responsavel)) {
				$where .= " AND usunome ilike '%{$responsavel}%' ";
			}
		}
		$sql = "
        SELECT usucpf, usunome, pflcod, pfldsc, sum(hoje) hoje, sum(posse) posse, sum(geral) geral
          FROM (
        -- Qtd para Hoje
        SELECT rea.usucpf, usu.usunome, p.pflcod, p.pfldsc, count(dmd.dmdid) hoje, 0 posse, 0 geral
        FROM demandasfies.demanda dmd
                INNER JOIN demandasfies.responsavelarea rea ON rea.dmdid = dmd.dmdid
                INNER JOIN seguranca.usuario usu ON usu.usucpf = rea.usucpf
                INNER JOIN workflow.documento doc on doc.docid = dmd.docid
                INNER JOIN seguranca.perfilusuario pu on pu.usucpf = usu.usucpf
                INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod AND p.sisid =  {$sisId}

        WHERE dmd.dmdstatus = 'A'
        AND reastatus = 'A'
        AND doc.esdid != 1275 -- DEMANDAS NAO FINALIZADAS
        and (
                (rea.esdid = 1271 AND doc.esdid IN (1271, 1296)) or -- Em elaborA??o ou com Procuradoria Federal
                (rea.esdid = doc.esdid)
        )
        AND rea.reaprazo = CURRENT_DATE
        GROUP BY  rea.usucpf, usu.usunome, p.pfldsc, p.pflcod

        UNION
        -- Em posse
        SELECT  rea.usucpf, usu.usunome, p.pflcod, p.pfldsc, 0, count(rea.usucpf), 0
        FROM demandasfies.demanda dmd
                INNER JOIN demandasfies.responsavelarea rea ON rea.dmdid = dmd.dmdid
                INNER JOIN seguranca.usuario usu ON usu.usucpf = rea.usucpf
                INNER JOIN workflow.documento doc on doc.docid = dmd.docid
                INNER JOIN seguranca.perfilusuario pu on pu.usucpf = usu.usucpf
                INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod  AND p.sisid =  {$sisId}
        WHERE dmd.dmdstatus = 'A'
        AND reastatus = 'A'
        AND doc.esdid != 1275 -- DEMANDAS NAO FINALIZADAS
        and (
                (rea.esdid = 1271 AND doc.esdid IN (1271, 1296)) or -- Em elaborA??o ou com Procuradoria Federal
                (rea.esdid = doc.esdid)
        )
        GROUP BY  rea.usucpf, usu.usunome, p.pfldsc, p.pflcod


        UNION
        -- Qtd Geral
        SELECT  rea.usucpf, usu.usunome, p.pflcod, p.pfldsc, 0, 0, count(rea.usucpf)
        FROM demandasfies.demanda dmd
                INNER JOIN demandasfies.responsavelarea rea ON rea.dmdid = dmd.dmdid
                INNER JOIN seguranca.usuario usu ON usu.usucpf = rea.usucpf
                INNER JOIN workflow.documento doc on doc.docid = dmd.docid
                INNER JOIN seguranca.perfilusuario pu on pu.usucpf = usu.usucpf
                INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod  AND p.sisid =  {$sisId}
        WHERE dmd.dmdstatus = 'A'
        AND reastatus = 'A'
        AND doc.esdid != 1275 -- DEMANDAS NAO FINALIZADAS
        GROUP BY  rea.usucpf, usu.usunome, p.pfldsc,  p.pflcod
) as foo
WHERE 1 = 1 {$where}
GROUP BY usucpf, usunome, pfldsc, pflcod
ORDER BY pfldsc,usunome
;
       ";

		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();

		$listagem = new Simec_Listagem();
		$listagem->setTamanhoPagina(50);
		$listagem->addCallbackDeCampo(array('usunome', 'pfldsc'), 'alinhaParaEsquerda');
		$listagem->addCallbackDeCampo(array('hoje', 'posse', 'geral'), 'exibirDemandasResposavel');
		$listagem->esconderColunas(array('docid', 'dmdprazo', 'acjid'));
		$listagem->setCabecalho(array('Responsável', 'Área', 'Qtd para Hoje', 'Em posse', 'Qtd Geral',));
		$listagem->esconderColunas(array('usucpf', 'pflcod'));
		$listagem->setDados($dados);
		$listagem->render();
	}

	public function getSqlListaRelatorioPorCampo($cpf, $campo)
	{
		switch ($campo) {
			case 'hoje':
				$where = '  AND (  (rea.esdid = 1271 AND doc.esdid IN (1271, 1296)) or -- Em elaboracao ou com Procuradoria Federal
                                        (rea.esdid = doc.esdid)
                                    )
                            AND rea.reaprazo = CURRENT_DATE ';
				break;
			case 'posse':
				$where = '  AND ( (rea.esdid = 1271 AND doc.esdid IN (1271, 1296)) or -- Em elaboracao ou com Procuradoria Federal
                                        (rea.esdid = doc.esdid)
                                    ) ';
				break;
			case 'geral':
				$where = '';
				break;
		}

		return "
        SELECT  dmd.dmdid, dmd.dmdid as codigo, obj.objnome,
         (coalesce(    public.formata_cpf_cnpj(usu.usucpf),'')  || ' - ' || usu.usunome ) as autor,
        dmdprocesso,
			case
                           when coalesce(rea.reaprazo::char, '') = '' then ''
                           when EXTRACT(DAYS FROM(rea.reaprazo - NOW()))::integer < 0
                               then '<span style=\"color: red;\">' || to_char(rea.reaprazo, 'DD/MM/YYYY') || '<br />' || EXTRACT(DAYS FROM(rea.reaprazo - NOW())) || ' dias </span>'
                           else to_char(rea.reaprazo, 'DD/MM/YYYY') || '<br />' || EXTRACT(DAYS FROM(rea.reaprazo - to_char(NOW(), 'YYYY-MM-DD')::date)) || ' dias'
			end as prazoarea,
			EXTRACT(DAYS FROM(NOW() - coalesce(hd.htddata, doc.docdatainclusao)))::integer as diassituacao,
			ed.esddsc, acj.acjnome, dmd.acjid, dmd.dmdprazo, doc.docid

        FROM demandasfies.demanda dmd
                INNER JOIN demandasfies.responsavelarea rea ON rea.dmdid = dmd.dmdid
                INNER JOIN seguranca.usuario usu ON usu.usucpf = rea.usucpf
                INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
                INNER JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
                LEFT JOIN workflow.historicodocumento hd on hd.hstid = doc.hstid
                INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = usu.usucpf
                INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND p.sisid =  198
		        LEFT  JOIN demandasfies.acaojudicial acj ON acj.acjid = dmd.acjid
                LEFT  JOIN demandasfies.objeto obj ON obj.objid = dmd.objid

        WHERE dm = 'A'
        AND reastatus = 'A'
        AND doc.esdid != 1275 -- DEMANDAS NAO FINALIZADAS
        {$where}
        AND rea.usucpf = '{$cpf}'
        ";
	}

	public function vincularMaterias($materias)
	{
		$sql = "update demandasfies.demandamateria set
                    dmmstatus = 'I',
                    usucpfinativacao = '{$_SESSION['usucpforigem']}',
                    dmmdtinativacao = NOW()
                where dmdid = {$this->dmdid}
                and dmmstatus  = 'A'";
		$this->executar($sql);

		$materias = is_array($materias) ? $materias : array();
		foreach ($materias as $matid) {
			$demandaMateria = new DemandaMateria();
			$demandaMateria->dmdid = $this->dmdid;
			$demandaMateria->matid = $matid;
			$demandaMateria->dmmdtinclusao = date('Y-m-d H:i:s');
			$demandaMateria->usucpfinclusao = $_SESSION['usucpforigem'];
			$demandaMateria->salvar();
			unset($demandaMateria);
		}
		$this->commit();
	}

	public function vincularClassificacao($classificacoes, $clatipo = 'G')
	{
		$sql = "update demandasfies.demandaclassificacao set
                    dmcstatus = 'I',
                    usucpfinativacao = '{$_SESSION['usucpforigem']}',
                    dmcdtinativacao = NOW()
                where dmdid = {$this->dmdid}
                and claid in (select claid from demandasfies.classificacao where clatipo = '{$clatipo}' )
                and dmcstatus  = 'A'";

		$this->executar($sql);

		$classificacoes = is_array($classificacoes) ? $classificacoes : array();

		foreach ($classificacoes as $claid) {
			$demandaClassificacao = new DemandaClassificacao();
			$demandaClassificacao->dmdid = $this->dmdid;
			$demandaClassificacao->claid = $claid;
			$demandaClassificacao->dmcdtinclusao = date('Y-m-d H:i:s');
			$demandaClassificacao->usucpfinclusao = $_SESSION['usucpforigem'];
			$demandaClassificacao->salvar();
			unset($demandaClassificacao);
		}
		$this->commit();
	}

	public function getDataRespProfeByDmdid($dmdid = null)
	{
		if (empty($dmdid)) {
			$dmdid = $this->dmdid;
		}
		$esd_em_cadastramento = ESD_DEMANDA_EM_CADASTRAMENTO;
		$sql = "
			SELECT   to_char(reaprazo, 'DD/MM/YYYY') as reaprazo
			FROM demandasfies.responsavelarea ra
			WHERE dmdid = {$dmdid} AND reastatus = 'A' AND esdid = {$esd_em_cadastramento}
		";

		$dados = $this->carregar($sql);
		if ($dados) {
			return $dados[0]['reaprazo'];
		}
		return false;
	}

	public function getTituloEEscondeColuna()
	{
		$tituloCabecalho = array('Código', 'Objeto', 'Autor', 'Responsável', 'Nº Processo', 'Dias em atraso', 'Dias na <br />Situação', 'Situação', 'Tipo Ação');
		$colunasEscondidas = array('docid', 'dmdprazo', 'acjid', 'prazo_resp', 'reaprazo', 'prazo_resp_prof', 'dias_em_atraso_profe', 'prazo_resp_advogado', 'prazo_resp_analista_dti', 'htddata_dti', 'htddata_analista_dti');

		$perfis = pegaPerfilGeral();

		if (in_array(PFL_NUCLEO_JURIDICO, $perfis)) {
			$tituloCabecalho = array('Código', 'Objeto', 'Autor', 'Responsável', 'Nº Processo', 'Dias em atraso', 'Dias na <br />Situação', 'Situação', 'Data Advogado', 'Tipo Ação', 'Prazo do responsável');
			$colunasEscondidas = array('docid', 'dmdprazo', 'acjid', 'reaprazo', 'prazo_resp_prof', 'dias_em_atraso_profe', 'prazo_resp_analista_dti', 'htddata_dti', 'htddata_analista_dti');
		}

		if (in_array(PFL_PROCURADOR_FEDERAL, $perfis)) {
			$tituloCabecalho = array('Código', 'Objeto', 'Autor', 'Responsável', 'Nº Processo', 'Dias em atraso', 'Dias na <br />Situação', 'Situação', 'Tipo Ação', 'Data PROFE');
			$colunasEscondidas = array('docid', 'dmdprazo', 'acjid', 'reaprazo', 'prazo_resp', 'dias_em_atraso_profe', 'prazo_resp_advogado', 'prazo_resp_analista_dti', 'htddata_dti', 'htddata_analista_dti');
		}

		if (in_array(PFL_DTI_MEC, $perfis) || in_array(PFL_ANALISTA_DTI_MEC, $perfis)) {
			$tituloCabecalho = array('Código', 'Objeto', 'Autor', 'Responsável', 'Nº Processo', 'Dias em atraso', 'Dias na <br />Situação', 'Situação', 'Tipo Ação', 'Data de envio DTI', 'Data de envio Analista DTI', 'Data prevista atendimento Analista DTI');
			$colunasEscondidas = array('docid', 'dmdprazo', 'acjid', 'prazo_resp', 'reaprazo', 'prazo_resp_prof', 'dias_em_atraso_profe', 'prazo_resp_advogado');
		}

		/** SE FOR ADVOGADO EXIBE O PRAZO DO RESPONSAVEL */
		if (in_array(PFL_ADVOGADO, $perfis)) {
			$tituloCabecalho = array('Código', 'Objeto', 'Autor', 'Responsável', 'Nº Processo', 'Dias em atraso', 'Dias na <br />Situação', 'Situação', 'Data Advogado', 'Tipo Ação', 'Data de envio DTI', 'Data de envio Analista DTI', 'Data prevista atendimento Analista DTI');
			$colunasEscondidas = array('docid', 'dmdprazo', 'acjid', 'reaprazo', 'prazo_resp', 'prazo_resp_prof', 'dias_em_atraso_profe');
		}

		return array('tituloCabecalho' => $tituloCabecalho, 'colunasEscondidas' => $colunasEscondidas);
	}

	public function utf8Encode(&$dados)
	{
		if (is_array($dados)) {
			foreach ($dados as &$dado) {
				if (is_array($dado)) {
					$this->utf8Encode($dado);
				} else {
					$dado = utf8_encode($dado);
				}
			}
		}
	}

	public function getSqlQuantidadeEmDistribuicaoValidacao()
	{
		$esdIdAdvogado = ESD_DEMANDA_ADVOGADO;
		$esdIdsProfeCadastramento = ESD_DEMANDA_PROFE . ',' . ESD_DEMANDA_EM_CADASTRAMENTO;
		$perfis = array(PFL_NUCLEO_JURIDICO);
		return "
					SELECT 'Distribuição' AS tipo, COUNT(DISTINCT d.dmdid) AS valor
						FROM demandasfies.demanda d
						INNER JOIN workflow.documento doc on doc.docid = d.docid
						LEFT JOIN workflow.historicodocumento hd on hd.hstid = doc.hstid
						INNER JOIN workflow.acaoestadodoc aed on aed.aedid = hd.aedid
						INNER JOIN workflow.estadodocumento ed on ed.esdid = doc.esdid
						INNER JOIN workflow.acaoestadodoc ae on ae.esdidorigem = ed.esdid
						INNER JOIN workflow.estadodocumentoperfil ep on ep.aedid = ae.aedid

						WHERE ep.pflcod in ( 1289 )
						AND aed.esdidorigem in({$esdIdsProfeCadastramento})

					UNION
					SELECT 'Validação' AS tipo, COUNT(DISTINCT d.dmdid) AS valor
						FROM demandasfies.demanda d
						INNER JOIN workflow.documento doc on doc.docid = d.docid
						LEFT JOIN workflow.historicodocumento hd on hd.hstid = doc.hstid
						INNER JOIN workflow.acaoestadodoc aed on aed.aedid = hd.aedid
						INNER JOIN workflow.estadodocumento ed on ed.esdid = doc.esdid
						INNER JOIN workflow.acaoestadodoc ae on ae.esdidorigem = ed.esdid
						INNER JOIN workflow.estadodocumentoperfil ep on ep.aedid = ae.aedid

						WHERE ep.pflcod in ( 1289 )
						AND aed.esdidorigem in( {$esdIdAdvogado} )
					";
	}
}