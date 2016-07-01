<?php

include APPRAIZ . "contratogestao/classes/html_table.class.php";

class Model_Hierarquiacontrato extends Abstract_Model
{

	protected $_schema = 'contratogestao';
	protected $_name = 'hierarquiacontrato';
	public $entity = array();
	private $tabelaArvore;
	private $contrato;
	private $fatorAvaliado;

	const NIVEL_CONTRATO = 'Contrato';
	const NIVEL_MACROPROCESSO = 'Macroprocesso';
	const NIVEL_SUBPROCESSO = 'Subprocesso';
	const NIVEL_PROCESSO = 'Processo';
	const NIVEL_FASE = 'Fase';
	const NIVEL_ACAO = 'Ação';
	const NIVEL_ATIVIDADE = 'Atividade';

	public function __construct($commit = true)
	{
		parent::__construct($commit);
		$this->tabelaArvore = new HTML_Table('arvore_contrato', 'table table-condensed table-bordered  table-responsive tabela_arvore_contrato');
		$this->contrato = new Model_Contrato($commit);
		$this->fatorAvaliado = new Model_Fatoravaliado($commit);

		$this->entity['hqcid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'ID');
		$this->entity['hqcidpai'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'ID Pai');
		$this->entity['hqcnivel'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Nível');
		$this->entity['hqcordem'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Ordem');
	}

	public function getArvore($where)
	{
		$dados = $this->getNos(false, $where);

		if ($dados) {
			$nivel = $dados[0]['level'];
			$hqcidpai = $dados[0]['hqcidpai'];
			
			$camposDaTabela = $this->getCamposTabela($nivel);

			
			$this->tabelaArvore = new HTML_Table("arvore_contrato{$hqcidpai}", "table table-condensed table-bordered  table-responsive tabela_arvore_contrato nivel_{$nivel}");
			$this->setHeader($camposDaTabela, $nivel);
			$this->setBody($dados, $camposDaTabela);

			$titulo = '';
			if ($nivel > 1) {
				$nivelDescricao = $this->getNivelDescricao($nivel);
				$titulo = "<br><h5 style='padding-left: 15px;'><b>{$nivelDescricao}</b></h5>";
			}

			return $titulo . $this->tabelaArvore->display();
		}

	}

	public function getCamposTabela($nivel)
	{
		if ($nivel == 1) {
			return array('condescricao', 'conprocesso', 'hqcnivel', 'datainicial', 'datafinal', 'conarearesponsavel', 'fatvalordesembolso');
		} else {
			return array('condescricao', 'datainicial', 'datafinal','fatvalordesembolso');
		}
	}

	public function setHeader($camposDaTabela, $nivel)
	{
		$this->tabelaArvore->addTSection('thead', 'thead');
		$this->tabelaArvore->addRow("theader_{$nivel}");
		$colspan = ($nivel == 7 ? 3 : 4);

		$this->tabelaArvore->addCell('Ações', '', 'header', array('class' => 'text-center', 'colspan' => $colspan));
		foreach ($camposDaTabela as $campo) {
			$label = $this->contrato->getAttributeLabel($campo);
			if (empty($label)) {
				$label = $this->getAttributeLabel($campo);
			}
			if (empty($label)) {
				$label = $this->fatorAvaliado->getAttributeLabel($campo);
			}
			$this->tabelaArvore->addCell($label, '', 'header', array('class' => 'text-center'));
		}
	}

	public function setBody($dados, $camposDaTabela)
	{
		$nivel = $dados[0]['hqcnivel'];
		$this->tabelaArvore->addTSection('tbody', "tbody body{$nivel}");

		// Realiza somatorio dos valores dos filhos.
		foreach ($dados as $contrato) {
			$classCssTr = " itemNivel{$contrato['level']} ";
			$possuiFilhos = '';

			if (!empty ($contrato['hqcidpai'])) {
				$possuiFilhos = ($this->possuiFilho($contrato['hqcid']) ? 'true' : 'false');
			}

			$htmlOptionsLinha = array('data-tt-conid' => $contrato['conid'], 'data-tt-id' => $contrato['hqcid'], 'data-tt-parent-id' => $contrato['hqcidpai'], 'data-tt-nivel' => $contrato['hqcnivel'], 'data-tt-raiz' => ($contrato['npai'] != $contrato['hqcid'] ? $contrato['npai'] : ''), 'data-tt-possui-filhos' => $possuiFilhos,);


			$classCssTr .= (empty($contrato['hqcidpai']) ? ' tr_raiz tr_item ' : ' tr_item ');
			$this->tabelaArvore->addRow($classCssTr, $htmlOptionsLinha);

			$this->getBotaoAcoes($contrato, $contrato['npai']);

			foreach ($camposDaTabela as $campo) {

				$valor = $this->tratarObjetoContrato($campo, $contrato, $dados);

				if (strpos($campo, 'condescricao') !== false) {
					$aditivo = ($contrato['conaditivo'] == 't' ? ' <span class="label label-danger">aditivo</span>' : '');
					$niveis = $this->getNivel();
					$nivelDescricao = $valor . $aditivo;
					$this->tabelaArvore->addCell($nivelDescricao, "nivel");
				} else {
					$this->tabelaArvore->addCell($valor);
				}
			}
		}
	}

	public function getBotaoAcoes($contrato)
	{
		$classeCssAdicionar = 'adicionar_contrato_item';
		$conid = $contrato['conid'];
		$hqcid = $contrato['hqcid'];
		$hqcnivel = $contrato['hqcnivel'];

		if (empty($contrato['hqcidpai'])) {
			$classeCssEditar = 'editarContrato';
			$classeCssOrdenar = false;
			$botaoRelatorio = "<a class='btn btn-default btn-xs relatorio' href='contratogestao.php?modulo=relatorio/dashboard&acao=A&conid={$conid}' " . "data-tt-hqcid='{$hqcid}' id='{$conid}'  title='Relatório Dashboard'><span class='glyphicon
			glyphicon-th-large'></span></button>";
			$this->tabelaArvore->addCell($botaoRelatorio, '', 'data', array('class' => 'text-center'));
		} else {
			$classeCssEditar = 'editar_item';
			$classeCssOrdenar = 'item_ordernar';
		}

		$botaoAtualizar = "<button class='btn btn-default btn-xs {$classeCssEditar}'    id='{$conid}' type='button'  title='Editar'><span class='glyphicon glyphicon-pencil'></span></button>";
		$botaoAdicionar = "<button class='btn btn-default btn-xs {$classeCssAdicionar}' id='{$conid}' type='button' title='Adicionar'><span class='glyphicon glyphicon-plus'></span></button>";
		$botaoExcluir = "<button class='btn btn-default btn-xs removerContrato' data-tt-hqcid = '{$hqcid}' id='{$conid}' type='button'  title='Excluir'><span class='glyphicon glyphicon-trash'></span></button>";
		$botaoOrdem = "<span class='glyphicon glyphicon-resize-vertical {$classeCssOrdenar}' title='Ordenar entre n?veis'></span>";

		$perfilUsuario = new Model_PerfilUsuario();
		if ($perfilUsuario->validarAcessoModificacao($conid) === false OR is_null($perfilUsuario->validarAcessoModificacao($conid))) {
			$botaoOrdem = $botaoExcluir = $botaoAdicionar = $botaoAtualizar = '&nbsp;&nbsp;&nbsp;';
		}

		$this->tabelaArvore->addCell($botaoAtualizar, '', 'data', array('class' => 'text-center'));
		$this->tabelaArvore->addCell($botaoExcluir, '', 'data', array('class' => 'text-center'));

		if ($hqcnivel != 7) {
			$this->tabelaArvore->addCell($botaoAdicionar, '', 'data', array('class' => 'text-center'));
		}
		if ($hqcnivel > 1) {
			$this->tabelaArvore->addCell($botaoOrdem, '', 'data', array('class' => 'text-center'));
		}
	}

	public function getLabelAtribute($atributo)
	{
		return $this->entity[$atributo]['label'];
	}

	public function getNivel()
	{
		return array(1 => self::NIVEL_CONTRATO, 2 => self::NIVEL_MACROPROCESSO, 3 => self::NIVEL_SUBPROCESSO, 4 => self::NIVEL_PROCESSO, 5 => self::NIVEL_FASE, 6 => self::NIVEL_ACAO, 7 => self::NIVEL_ATIVIDADE);
	}

	public function getNivelDescricao($id)
	{
		$niveis = $this->getNivel();
		return $niveis[$id];
	}

	public function getOptionNivel()
	{
		$optionHtml = '<option value="">.. selecione ..</option>';
		foreach ($this->getNivel() as $indice => $nivel) {
			$optionHtml .= "<option value='{$indice}'>{$nivel}</option>";
		}
		return $optionHtml;
	}

	public function possuiFilho($idPai, $dados = array())
	{
		if (!empty($dados)) {
			foreach ($dados as $contrato_) {
				if ($contrato_['hqcidpai'] === $idPai) {
					return true;
				}
			}
			return false;
		} else {
			$nosFilhos = $this->getNos($idPai);
			return !empty($nosFilhos);
		}
	}

	public function setDataInsert($dados)
	{
		$this->entity['hqcidpai']['value'] = $dados['hqcid'];
		$this->entity['hqcnivel']['value'] = $dados['hqcnivel'] + 1;
	}

	public function alterarNiveisNosFilhos($idPai, $nivel)
	{
		$nosFilhos = $this->getNos($idPai);
		$valida = true;
		$hierarquiaContrato = new Model_Hierarquiacontrato(false);

		if (is_array($nosFilhos)) {
			foreach ($nosFilhos as $no) {
				$hierarquiaContrato->clearEntity();
				$hierarquiaContrato->populateEntity($no);
				$hierarquiaContrato->entity['hqcnivel']['value'] = $nivel + $no['level'];
				if ($hierarquiaContrato->getAttributeValue('hqcnivel') > 7) {
					$valida = false;
					break;
				} else {
					$hierarquiaContrato->save();
				}
			}
		}
		if ($valida) {
			$hierarquiaContrato->commit();
			return true;
		} else {
			$hierarquiaContrato->rollback();
			return false;
		}
	}

	public function getHierarquiaContratoById($id)
	{
		$sql = "SELECT hqcid, hqcidpai, hqcnivel, hqcordem  FROM contratogestao.hierarquiacontrato  WHERE hqcid = {$id} ";
		$dados = $this->_db->carregar($sql);
		return $dados[0];
	}

	public function getNos($hqcidpai = false, $where = '')
	{
		if ($hqcidpai === false) {
			$idPaiSql = 'is null';
		} else {
			$idPaiSql = ' = ' . $hqcidpai;
		}
		$sql = "
             WITH RECURSIVE q AS
              (
                  SELECT  h,
                  			1 AS level,
                  			ARRAY[hqcid] AS breadcrumb,
                  			ARRAY[hqcid] AS breadcrumb_hqcid,
                  			hqcid as npai
                  FROM    contratogestao.hierarquiacontrato h
                  WHERE   hqcidpai  {$idPaiSql}
                  UNION ALL
                  SELECT  hi, q.level + 1 AS level,
                  		breadcrumb || hqcordem,
                 	 	breadcrumb_hqcid || hqcid breadcrumb_hqcid,
                 	 	npai
                  FROM    q
                  JOIN    contratogestao.hierarquiacontrato hi
                  ON      hi.hqcidpai = (q.h).hqcid
              )
              SELECT
                  REPEAT('   ', level) || (q.h).hqcid as arvore,
                  level,
                  breadcrumb::VARCHAR AS path,
                  breadcrumb_hqcid::VARCHAR AS breadcrumb_hqcid,
                  (q.h).*,
                  c.condescricao as descricao,
                  c.consigla || ' - ' || c.condescricao as condescricao,
                  c.conid, c.hqcid, c.datainicial, c.datafinal, c.consigla,
          c.conmeta, conpeso, c.concontratada, c.conprocesso, c.conobjetivo, c.connumerocontrato,
          c.connumeroaditivo, c.conarearesponsavel, c.conaditivo,
              npai
              FROM q
              INNER JOIN contratogestao.contrato c ON (q.h).hqcid = c.hqcid
              WHERE (q.h).hqcstatus = 'A'
              $where
              ORDER BY breadcrumb
            ";
//        ver($sql, d);
		$dados = $this->_db->carregar($sql);
//		ver($dados, d);
		return $dados;
	}

	public function getNosComPai($hqcid = false, $where = '')
	{
		if ($hqcid === false) {
			$idPaiSql = ' hqcidpai is null';
		} else {
			$idPaiSql = " hqcid = {$hqcid} ";
		}

		$sql = "  WITH RECURSIVE q AS
            (
                SELECT  h, 1 AS level,
					ARRAY[hqcid] AS breadcrumb,
					ARRAY[hqcid] AS breadcrumb_hqcid,
					hqcid as npai
                FROM    contratogestao.hierarquiacontrato h
                WHERE   {$idPaiSql}
                UNION ALL
                SELECT  hi,
					q.level + 1 AS level,
					breadcrumb || hqcordem,
					breadcrumb_hqcid || hqcid breadcrumb_hqcid,
					npai
                FROM    q
                JOIN    contratogestao.hierarquiacontrato hi ON hi.hqcidpai = (q.h).hqcid
            )
            SELECT
                REPEAT('   ', level) || (q.h).hqcid as arvore,
                level,
                breadcrumb::VARCHAR AS path,
                breadcrumb_hqcid::VARCHAR AS breadcrumb_hqcid,
                (q.h).*,
                c.condescricao as descricao,
                c.consigla || ' - ' || c.condescricao as condescricao,
                c.conid, c.hqcid, c.datainicial, c.datafinal, c.consigla,
                c.conmeta, conpeso, c.concontratada, c.conprocesso, c.conobjetivo, c.connumerocontrato,
                c.connumeroaditivo, c.conarearesponsavel, c.conaditivo,
            npai
            FROM q
            INNER JOIN contratogestao.contrato c ON (q.h).hqcid = c.hqcid
            WHERE (q.h).hqcstatus = 'A'
             {$where}
            ORDER BY breadcrumb
            ";
//        ver($sql);
		$dados = $this->_db->carregar($sql);
		return $dados;
	}

	public function getEnderecoArvoreContrato($idhPai, $hqcid)
	{
		$contratosArray = array();

		$idhPai = (int)$idhPai;
		$dadosFilho = $this->getNos(false, " AND (q.h).hqcid = {$idhPai} ");
		$dadosFilho = $dadosFilho[0];

		$itemDoPai = $this->getNosComPai($dadosFilho['npai']);
		$itemDoPai = array_reverse($itemDoPai);

		$hqcidpai = $dadosFilho['hqcidpai'];

		foreach ($itemDoPai as $contrato) {
			if ($hqcidpai == $contrato['hqcid'] OR $dadosFilho['npai'] == $contrato['hqcid'] OR $hqcid == $contrato['hqcid']) {
				$contratosArray[] = $contrato['condescricao'];
				$hqcidpai = $contrato['hqcidpai'];
			}
		}

		$contratosArray = array_reverse($contratosArray);
		$path = (implode(' &#187; ', $contratosArray));
		return $path;
	}

	public function verificaVinculos($hqcid, $dados = array())
	{
		$contrato = new Model_Contrato();
		if (!$this->possuiFilho($hqcid, $dados) && !$contrato->possuiFatorAvaliado($hqcid)) {
			return false;
		}
		return true;
	}

	public function countItemFilhos($idPai)
	{
		$hierarquiacontrato = new Model_Hierarquiacontrato();
		$dados = $hierarquiacontrato->getAllByValues(array('hqcidpai' => (int)$idPai, 'hqcstatus' => 'A'));
		return count($dados) + 1;
	}

	public function getOptionsContrato($nivel = 1)
	{
		$nivel = (int)$nivel;
		$dados = $this->getNos(false, " AND (q.h).hqcnivel = {$nivel}");
		$option = "<option value=''> Todos </option>";
		if ($dados) {
			foreach ($dados as $item) {
				$selected = ((int)$item['conid'] === (int)$this->getAttributeValue('conid') ? 'selected=selected' : '');
				$option .= "<option value='{$item['conid']}' {$selected} >{$item['condescricao']}</option>";
			}
		}
		return $option;
	}

	public function tratarObjetoContrato($campo, $contrato, $dados)
	{
		$conid = $contrato['conid'];
		$valor = $contrato[$campo];
		$hqcid = $contrato['hqcid'];

		if (strpos($campo, 'data') !== false && !empty($valor) ) {
			$valor = date('d/m/Y', strtotime($valor));
		}

		if ( $campo== 'hqcnivel' ) {
			$valor = $this->getNivelDescricao($valor);
		}

		if ( $campo == 'fatvalordesembolso') {
			$valor = $this->getSqlValorFatorAvaliado($hqcid);
			$valor = 'R$ ' . number_format($valor, 2, ',', '.');
			$class = 'text-right';
		}

		$paddingLeft = 'padding-left: 15px;';

		if ((int)$contrato['hqcnivel'] === 7 && $campo === 'condescricao') {
			$valor = " <span style='padding-left: 15px;'></span> <a href='contratogestao.php?modulo=inicio&id={$conid}&acao=A' class='fator_avaliado' id='{$conid}' title='Gerenciar Fator Avaliado'> {$valor}</a>";
		} elseif (((int)$contrato['level'] == 1 || $this->possuiFilho($contrato['hqcidpai'], $dados)) && $campo === 'condescricao') {
			$valor = "<span class='glyphicon glyphicon-chevron-down exibirItem exibirItem{$hqcid}' aria-hidden='true' style='{$paddingLeft}'></span> {$valor}";
		}
		return $valor;
	}

	public function getSqlValorFatorAvaliado($noPai)
	{
		$sql = "
				WITH RECURSIVE qv AS
				(
					SELECT
						h1,
						ARRAY[hqcid] AS breadcrumb
					FROM    contratogestao.hierarquiacontrato h1
					WHERE   hqcidpai is null
				UNION ALL
					SELECT
						h2,
						breadcrumb || hqcid
					FROM    qv
					JOIN    contratogestao.hierarquiacontrato h2
					ON      h2.hqcidpai = (qv.h1).hqcid
				)
				SELECT sum(fatvalordesembolso) as fatvalordesembolso
				FROM qv
				INNER JOIN contratogestao.contrato contrato ON (qv.h1).hqcid = contrato.hqcid
				INNER JOIN contratogestao.fatoravaliado fatorAvaliado ON fatorAvaliado.conid = contrato.conid
				WHERE  fatorAvaliado.fatstatus = 'A'
					AND breadcrumb &&  ARRAY[ {$noPai}  ]
					AND fatorAvaliado.fatvalordesembolso IS NOT NULL
		";
		$dados = $this->_db->carregar($sql);
		if(!empty($dados) && isset($dados[0]['fatvalordesembolso'])){
			return $dados[0]['fatvalordesembolso'];
		}
		return 0;
	}

	/**
	 * Faz a conta dos valores filhos com os pais.
	 *
	 * @param array $father
	 * @return array $father
	 */
	function sumTotalContrato(&$father)
	{
		$father = array_reverse($father);
		foreach ($father as &$son) {
			$sum = (float)$son['fatvalordesembolso'];
			foreach ($father as $values) {
				if ($son['hqcid'] == $values['hqcidpai']) {
					$sum += (float)$values['fatvalordesembolso'];
				}
			}
			if (empty($son['fatvalordesembolso'])) {
				$son['fatvalordesembolso'] = $sum;
			}
		}
		$father = array_reverse($father);
	}
}
