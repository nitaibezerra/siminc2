<?php
/**
 * Verifica se determinado usuário possui o perfil informado no parâmetro
 * @param $perfil -- ID do perfil
 * @return bool
 */
function possui_perfil($perfil) {
    global $db;

    if (!is_array($perfil)) {
        $perfil = array($perfil);
    }

    $strSQL = "
        SELECT
            count(1)
        FROM
            seguranca.perfilusuario p
            JOIN ted.usuarioresponsabilidade u ON (u.usucpf = p.usucpf)
        WHERE
            p.usucpf = '{$_SESSION['usucpf']}'
            AND p.pflcod in (".implode(',',$perfil).")
            AND u.rpustatus = 'A'
    ";
    //ver($strSQL, d);
    return (boolean) $db->pegaUm($strSQL);
}

function possui_perfil_gestor($perfil) {
    global $db;

    if (!is_array($perfil)) {
        $perfil = array($perfil);
    }

    $strSQL = "
        SELECT
            count(1)
        FROM
            seguranca.perfilusuario p
        WHERE
            p.usucpf = '{$_SESSION['usucpf']}'
            AND p.pflcod in (".implode(',',$perfil).")
    ";
    //ver($strSQL, d);
    return (boolean) $db->pegaUm($strSQL);
}

/**
 * Pega o id do documento do plano de trabalho
 *
 * @param integer $lbrid
 * @return integer
 */
function pegarDocid($tcpid) {
    global $db;

    if ($tcpid) {
        $strSQL = "Select	docid
				From ted.termocompromisso
				Where tcpid = $tcpid
		";
        //ver($sql, d);
        return $db->pegaUm($strSQL);
    }
    return false;
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $lbrid
 * @return integer
 */
function pegarEstadoAtual($tcpid, $retornarDescricao = false) {
    global $db;

    $docid = pegarDocid($tcpid);

    if ($docid) {
        $sql = <<<DML
SELECT ed.esdid,
       ed.esddsc
  FROM workflow.documento d
    INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
  WHERE d.docid = {$docid}
DML;
        $esddoc = $db->carregar($sql);
        if ($esddoc) {
            if (!$retornarDescricao) {
                return (integer)$esddoc[0]['esdid'];
            }
            return array((int)$esddoc[0]['esdid'], $esddoc[0]['esddsc']);
        }
    }
    return false;
}

/**
 * Verifica se possuí responsabilidade no modulo
 */
function possuiResponsabilidade($usucpf, $pflcod) {
    global $db;

    $strSQL = "
        SELECT DISTINCT ungcod
        FROM ted.usuarioresponsabilidade
        WHERE
            usucpf = '{$usucpf}'
            AND pflcod in (".implode(',',$pflcod).")
    ";

    return $db->pegaUm($strSQL);
}

function listaPendenciasTed($tcpid) {
    global $db;
    require_once APPRAIZ . 'includes/workflow.php';

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
    $estadoAtual = pegarEstadoAtual($tcpid);
    $boMostraWorkflow = true;

    //Termo aguardando aprovação do Gestor Orçamentário do Proponente
    //Termo aguardando aprovação do Representante Legal do Proponente
    //Termo aguardando aprovação pelo Representante Legal do Concedente
    //Termo em Análise pelo Gestor Orçamentário do Concedente
    $arEsdidPropConc = array(
        TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP,
        EM_APROVACAO_DA_REITORIA,
        AGUARDANDO_APROVACAO_SECRETARIO,
        EM_ANALISE_PELA_SPO
    );

    if (in_array($estadoAtual, $arEsdidPropConc)) {
        $boMostraWorkflow = false;

        $sql = "
            select
                ungcodproponente,
                ungcodconcedente
            from ted.termocompromisso
            where tcpid = {$tcpid}
        ";

        $rsUgConcPropTermo = $db->pegaLinha($sql);

        if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
            $boMostraWorkflow = true;
        }

        $rco = new Ted_Model_RelatorioCumprimento_Business();
        if (in_array(UO_EQUIPE_TECNICA, $perfis) && $rco->termoVencido($tcpid)) {
            $boMostraWorkflow = true;
        }

        // estado = Termo aguardando aprovação do Representante Legal do Proponente
        // perfil = Representante Legal do Proponente
        else if ($estadoAtual == EM_APROVACAO_DA_REITORIA && in_array(PERFIL_REITOR, $perfis)) {
            $ungcod = possuiResponsabilidade($_SESSION['usucpf'], $perfis);
            if ($rsUgConcPropTermo['ungcodproponente'] == $ungcod) {
                $boMostraWorkflow = true;
            }
        }

        // estado = Termo aguardando aprovação pelo  Representante Legal do Concedente
        // perfil = pflcod = Representante Legal do Concedente
        else if ($estadoAtual == AGUARDANDO_APROVACAO_SECRETARIO && in_array(PERFIL_SECRETARIO, $perfis)) {
            $ungcod = possuiResponsabilidade($_SESSION['usucpf'], $perfis);
            if($rsUgConcPropTermo['ungcodconcedente'] == $ungcod) {
                    $boMostraWorkflow = true;
            }
        }

        // estado = Termo aguardando aprovação do Gestor Orçamentário do Proponente
        // perfil = pflcod = Gestor Orçamentário do Proponente
        else if ($estadoAtual == TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP && in_array(PERFIL_PROREITOR_ADM, $perfis)) {
            $ungcod = possuiResponsabilidade($_SESSION['usucpf'], $perfis);
            if ($rsUgConcPropTermo['ungcodproponente'] == $ungcod) {
                $boMostraWorkflow = true;
            }
        }

        // estado = Termo em Análise pelo Gestor Orçamentário do Concedente
        // perfil = Gestor Orçamentário do Concedente
        else if ($estadoAtual == EM_ANALISE_PELA_SPO && in_array(PERFIL_SUBSECRETARIO, $perfis)) {
            $ungcod = possuiResponsabilidade($_SESSION['usucpf'], $perfis);
            if($rsUgConcPropTermo['ungcodconcedente'] == $ungcod) {
                $boMostraWorkflow = true;
            }
        }
    }

    $sql = "SELECT DISTINCT

				CASE WHEN ungcodproponente IS NOT NULL
					THEN true
					ELSE false
				END as abaproponente,

				CASE WHEN ungcodconcedente IS NOT NULL
					THEN true
					ELSE false
				END as abaconcedente,

				CASE WHEN
				    (SELECT true FROM ted.justificativa j WHERE j.tcpid = tcp.tcpid AND j.identificacao IS NOT NULL AND j.objetivo IS NOT NULL)
					THEN true
					ELSE false
				END as abadescentralizacao,

				CASE WHEN
					( select count(*) from ted.previsaoorcamentaria po06 where po06.tcpid = tcp.tcpid AND po06.prostatus = 'A'
									and po06.ndpid is not null
									and po06.provalor is not null)
					=
					( select count(*) from ted.previsaoorcamentaria po05 where po05.tcpid = tcp.tcpid AND po05.prostatus = 'A' )
					THEN true
					ELSE false
				END as abaprevisao,

				CASE WHEN
				    (select true from ted.parecertecnico where tcpid = tcp.tcpid
				      AND
						considentproponente   IS NOT NULL AND
						considproposta  	  IS NOT NULL AND
						considobjeto  		  IS NOT NULL AND
						considobjetivo  	  IS NOT NULL AND
						considjustificativa   IS NOT NULL AND
						considvalores  		  IS NOT NULL AND
						considcabiveis  	  IS NOT NULL
				    )
					THEN true
					ELSE false
				END as abaparecertecnico,

				CASE WHEN
					( select count(*) from ted.previsaoorcamentaria po03 where po03.tcpid = tcp.tcpid
									AND po03.prostatus = 'A'
									and po03.ptrid is not null
									and po03.pliid is not null
									and po03.crdmesliberacao is not null )
					=
					( select count(*) from ted.previsaoorcamentaria po02 where po02.tcpid = tcp.tcpid AND po02.prostatus = 'A' )
					THEN true
					ELSE false
				END as abaprevisaoanalise,

				( select count(recid) from ted.relatoriocumprimento rec where rec.tcpid = tcp.tcpid ) as relcumprimento,

				CASE WHEN apo.arqid IS NOT NULL
					THEN true
					ELSE false
				END as abaanexo,
				CASE WHEN (SELECT count(*) FROM ted.aditivovigencia v WHERE v.tcpid = tcp.tcpid) = 0 THEN
					FALSE
					ELSE TRUE
                END AS vigencia
			FROM
				ted.termocompromisso tcp
			LEFT JOIN ted.arquivoprevorcamentaria apo ON apo.tcpid = tcp.tcpid AND apo.arptipo = 'A'
			WHERE
				tcp.tcpid = {$tcpid}";
    //ver($sql, d);
    $arrValida = $db->pegaLinha($sql);

    $arrValida['abaproponente']			= $arrValida['abaproponente'] == 't' 		? true : false;
    $arrValida['abaconcedente'] 		= $arrValida['abaconcedente'] == 't' 		? true : false;
    $arrValida['abadescentralizacao'] 	= $arrValida['abadescentralizacao'] == 't' 	? true : false;
    $arrValida['abaprevisao']			= $arrValida['abaprevisao'] == 't' 			? true : false;
    $arrValida['abaparecertecnico']		= $arrValida['abaparecertecnico'] == 't' 	? true : false;
    $arrValida['abaprevisaoanalise']	= $arrValida['abaprevisaoanalise'] == 't' 	? true : false;
    $arrValida['relcumprimento']		= $arrValida['relcumprimento']>0	 		? true : false;
    $arrValida['abaanexo']				= $arrValida['abaanexo'] == 't'				? true : false;
    $arrValida['vigencia']				= $arrValida['vigencia'] == 't'				? true : false;

    $verificaEmcadastramento = false;
    if ($arrValida['abaproponente'] && $arrValida['abaconcedente'] && $arrValida['abadescentralizacao']
        && $arrValida['abaprevisao'] && $arrValida['abaanexo'] &&
        ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA))
    {
        $verificaEmcadastramento = true;
    }

    $verificaEmanalise = false;
    if ($arrValida['abaprevisaoanalise'] && $arrValida['abaparecertecnico'] && $estadoAtual == EM_ANALISE_OU_PENDENTE) {
        $verificaEmanalise = true;
    }

    $verificaOutroestado = false;
    if (!in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_DILIGENCIA, EM_ANALISE_OU_PENDENTE, EM_EXECUCAO, EM_DESCENTRALIZACAO))) {
        $verificaOutroestado = true;
    }

    $verificaEmexecucao = false;
    if ($arrValida['relcumprimento'] && $estadoAtual == EM_EXECUCAO) {
        $verificaEmexecucao = true;
    }

    $docid = pegarDocid($_GET['ted']);
    ?>

    <?php if($verificaOutroestado || $verificaEmanalise || $verificaEmcadastramento || $verificaEmexecucao) : ?>
        <tr>
            <td align="center">
                <br/>
                <b> Não possui pendências </b>
                <br/>
                <br/>
                <?php

                include_once APPRAIZ . 'includes/funcoesspo_componentes.php';

                // Monta combo das coordenações
                if($estadoAtual == EM_ANALISE_DA_SECRETARIA && ( in_array(PERFIL_SECRETARIA, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
                    $sql = "select ungcodconcedente, cooid, dircodpoliticafnde, ungcodpoliticafnde from ted.termocompromisso where tcpid = {$tcpid}";
                    $dado = $db->pegalinha($sql);

                    if($dado['ungcodconcedente'] == UG_FNDE && !empty($dado['dircodpoliticafnde'])){
                        $sql = "select cooid as codigo, coodsc as descricao from ted.coordenacao where dircod = '{$dado['dircodpoliticafnde']}' order by coodsc";
                    }else if($dado['ungcodconcedente'] == UG_FNDE && !empty($dado['ungcodpoliticafnde'])){
                        $sql = "select cooid as codigo, coodsc as descricao from ted.coordenacao where ungcodconcedente = '{$dado['ungcodpoliticafnde']}' order by coodsc";
                    }else{
                        $sql = "select cooid as codigo, coodsc as descricao from ted.coordenacao where ungcodconcedente = '{$dado['ungcodconcedente']}' order by coodsc";
                    }

                    $arrOptions = array(
                        'acao' => 'salvaCoordenacao'
                    );

                    if($db->pegaUm($sql)){
                        echo '<b>Selecione uma Coordenação </b><br/>';
                        echo inputCombo('cooid', $sql, $dado['cooid'], 'cooid', $arrOptions);
                        echo "<br><br>";
                    }
                    else{
                        echo '<b><font color=red>É necessário preencher a aba Concedente para selecionar uma Coordenação.</font></b><br><br>';
                        echo '<b>Selecione uma Coordenação </b><br/>';
                        echo inputCombo('cooid', $sql, $dado['cooid'], 'cooid', $arrOptions);
                        echo "<br><br>";
                    }
                }
                ?>
                <?php if($boMostraWorkflow = true): //a pedido do Werter ?>
                    <?php echo wf_desenhaBarraNavegacao($docid , array('docid' => $docid, 'tcpid' => $_REQUEST['ted']),  array('historico' => false)); ?>
                <?php else: ?>
                    <script type="text/javascript">

                        function wf_atualizarTela( mensagem, janela )
                        {
                            janela.close();
                            enviarFormulario();
                        }

                        function wf_alterarEstado( aedid, docid, esdid, acao )
                        {
                            if ( !confirm( 'Deseja realmente Salvar e ' + acao + ' ?' ) ){
                                return;
                            }
                            if(!validarFormularioPrincipal()){
                                return;
                            }
                            var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/alterar_estado.php' +
                                '?aedid=' + aedid +
                                '&docid=' + docid +
                                '&esdid=' + esdid +
                                '&verificacao=<?php echo urlencode( $dadosHtml ); ?>';
                            var janela = window.open(
                                url,
                                'alterarEstado',
                                'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
                            );
                            janela.focus();
                        }

                        function wf_exibirHistorico( docid )
                        {
                            var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
                                '?modulo=principal/tramitacao' +
                                '&acao=C' +
                                '&docid=' + docid;
                            window.open(
                                url,
                                'alterarEstado',
                                'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
                            );
                        }

                    </script>
                    <table border="0" cellspacing="0" cellpadding="3" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
                        <tbody>
                        <tr style="background-color: #c9c9c9; text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
								<span title="estado atual">
									<b>estado atual</b>
								</span>
                            </td>
                        </tr>
                        <tr style="text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
								<span title="estado atual">
									<?php
                                    $sql = "select esddsc from ted.termocompromisso tcp
									join workflow.documento doc on doc.docid = tcp.docid
									join workflow.estadodocumento esd on esd.esdid = doc.esdid
									where tcp.tcpid = {$tcpid}";
                                    echo $db->pegaUm($sql);
                                    ?>
								</span>
                            </td>
                        </tr>
                        <tr style="background-color: #c9c9c9; text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
								<span title="estado atual">
									<b>ações</b>
								</span>
                            </td>
                        </tr>
                        <tr>
                            <td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
                                nenhuma ação disponível para o documento
                            </td>
                        </tr>
                    </table>
                    <br/><br/>
                <?php endif; ?>
            </td>
        </tr>
    <?php else: ?>
        <tr class="well">
            <td>
                <strong>Pendência(s)</strong><br/>
            </td>
        </tr>
        <?php $boPendencia = false; ?>
        <?php if(!$arrValida['relcumprimento'] && $estadoAtual == EM_EXECUCAO): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao"">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/relatoriocuprimentoobjeto&acao=A&ted=<?=$tcpid?>">- Relatório de cumprimento.</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if(!$arrValida['abaparecertecnico'] && $estadoAtual == EM_ANALISE_OU_PENDENTE && ( in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/parecer&acao=A&ted=<?=$tcpid?>">- Parecer Técnico (Entidade Concedente).</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if(!$arrValida['abaprevisaoanalise'] && $estadoAtual == EM_ANALISE_OU_PENDENTE && ( in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/previsao&acao=A&ted=<?=$tcpid?>">- Programação Orçamentária.</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaproponente'] ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/proponente&acao=A&ted=<?=$tcpid?>">- Proponente.</>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaconcedente'] ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/concedente&acao=A&ted=<?=$tcpid?>">- Concedente.</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abadescentralizacao'] ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/justificativa&acao=A&ted=<?=$tcpid?>">- Objeto e Justificativa do Crédito.</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaprevisao'] && ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/previsao&acao=A&ted=<?=$tcpid?>">- Programação Orçamentária.</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaanexo'] && ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/anexos&acao=A&ted=<?=$tcpid?>">- Anexo.</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if(!$arrValida['vigencia'] && ($estadoAtual == EM_DESCENTRALIZACAO)) : ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:left;" class="botao">
                        <a href="ted.php?modulo=principal/termoexecucaodescentralizada/vigencia&acao=A&ted=<?=$tcpid?>">- Vigência.</a>
                    </div>
                </td>
            </tr>
        <?php endif; ?>

        <?php if($boMostraWorkflow): ?>
            <tr>
                <td align="center">
                    <?php echo wf_desenhaBarraNavegacao($docid, array('docid' => $docid, 'tcpid' => $_REQUEST['ted']), array('historico'=>false)); ?>
                </td>
            </tr>
        <?php endif; ?>
    <?php
    endif;
}

/**
 * Em teste - para novo relatório XLS
 * Cria celular somente dos lotes selecionados
 * @return bool
 */
function criaLoteMacroNCTeste() {
	global $db;

	if (is_array($_REQUEST['proid'])) {

		$_tcpid = array();
		if (count($_REQUEST['proid'])) {
			foreach ($_REQUEST['proid'] as $value) {
				$_tmpValue = explode('-', $value);
				if (!empty($_tmpValue[0]) && !in_array($_tmpValue[0], $_tcpid)) {
					array_push($_tcpid, $_tmpValue[0]);
				}
			}
		}

        //ver($_tcpid, $_POST, d);

		if (count($_tcpid)) {
			$sql = "select count(*) from ted.lotemacroitens where tcpid in (".implode(',', $_tcpid).")";
			$count = $db->pegaUm($sql);

			if ($count == count($_tcpid))
				return true;

			$sql = "insert into ted.lotemacro (lotdsc, lotdata, lotstatus, lotcpfresponsavel)
                    values ('Termos: ".implode(', ', $_tcpid)."', now(), 'A', '{$_SESSION['usucpf']}') returning lotid;";
			$lotid = $db->pegaUm($sql);

			$sqlItem = '';
			foreach ($_REQUEST['proid'] as $p) {
				$param = explode('-', $p);
				$_tcpid = $param[0];
				$_proid = $param[1];

				$existe = $db->pegaUm("select pro.proid from ted.previsaoorcamentaria pro where pro.tcpid = {$_tcpid} and pro.proid = {$_proid} and pro.prostatus = 'A'");
				if ($existe) {
					$sqlItem .= "insert into ted.lotemacroitens (lotid, tcpid, loistatus, proid) values ($lotid, $_tcpid, 'A', $_proid);";
				}
			}

			if ($sqlItem) {
				$db->executar($sqlItem);
				if ($db->commit()) {
					return true;
				} else {
                    $db->rollback();
                    return false;
                }
			}
		}
	}
	return false;
}

function contaQtdMaxPrevisaoOrcamentariaTeste()
{
	global $db;

	if(is_array($_REQUEST['proid'])){

		$_proid = array();
		foreach ($_REQUEST['proid'] as $tcpidProid) {
			$_param = explode('-', $tcpidProid);
			if (!in_array($_param[1], $_proid))
				array_push($_proid, $_param[1]);
		}

		$where[] = "pro.proid in (".implode(',', $_proid).")";
	}
	if($_REQUEST['lotid']){
		$where[] = "pro.proid in (select proid from ted.lotemacroitens where lotid = {$_REQUEST['lotid']})";
		$where[] = "pro.proid in (select loi.proid from ted.lotemacroitens loi where loi.proid = pro.proid and loi.loistatus = 'A')";
	}
	if(empty($_GET['lotid'])){
		$where[] = "doc.esdid in ( ".EM_DESCENTRALIZACAO.", ".EM_ANALISE_PELA_CGSO." )";
	}

	$sql = "SELECT max(total) as qtd FROM (
				SELECT
					count(*) AS total,
					tcp.tcpid
				FROM ted.termocompromisso tcp
				JOIN ted.previsaoorcamentaria pro ON pro.tcpid = tcp.tcpid
				JOIN workflow.documento doc ON doc.docid = tcp.docid
				".(is_array($where) ? ' WHERE '.implode(' AND ',$where) : '')."
				GROUP BY tcp.tcpid
			) AS foo";

	//ver($sql, d);
	return $db->pegaUm($sql);
}

function recuperarLoteTermosTeste() {
	global $db;

	if (is_array($_REQUEST['proid'])) {

		$_tcpid = array();
		$_proid = array();
		foreach ($_REQUEST['proid'] as $tcpidProid) {
			$_param = explode('-', $tcpidProid);

			if (!in_array($_param[0], $_tcpid))
				array_push($_tcpid, $_param[0]);

			if (!in_array($_param[1], $_proid))
				array_push($_proid, $_param[1]);
		}

		$where[] = "tcp.tcpid in (".implode(",", $_tcpid).")";

		$conditionWhere = "AND prot.proid in (".implode(",", $_proid).")";

	} else {
		if ($_REQUEST['lotid']) {

			//Pega um lote que já foi gerado
			$where[] = "po.proid in (
			select distinct po1.proid from ted.previsaoorcamentaria po1
			join ted.lotemacroitens loi on po1.proid = loi.proid
			where loi.lotid = {$_REQUEST['lotid']}
			)";

			$conditionWhere = "AND lotid = {$_REQUEST['lotid']}";
			$_lotid = (int) $_REQUEST['lotid'];

		} else {
		//Pega todas as celular orçamentárias que ainda não foram gerados lotes
			$where[] = "tcp.tcpid in (
			select distinct tcpid from ted.previsaoorcamentaria
			where prostatus = 'A' AND proid not in (select proid from ted.lotemacroitens where loistatus = 'A')
					)";
		            $where[] = "doc.esdid in ( ".EM_DESCENTRALIZACAO.", ".EM_ANALISE_PELA_CGSO." )";

					//Excluir as celulas orçamentarias que já foram descentralizadas em outro momento
					//$where[] = "po.proid in (select proid from monitora.previsaoorcamentaria where prostatus = 'A' and proid not in (select proid from elabrev.previsaoparcela pp where pp.ppacancelarnc = 'f'))";

		            $conditionWhere = "AND prot.tcpid = tcp.tcpid";
		            $_lotid = "(select lotid from ted.lotemacro order by lotid desc limit 1)";
		}
	}

    $emAnalisePelaCGSO = EM_ANALISE_PELA_CGSO;
    $aguardandoAprovacaoSecretario = AGUARDANDO_APROVACAO_SECRETARIO;

	$whereStr = '';
	if (is_array($where)) {
	$whereStr = ' WHERE ' . implode(' AND ',$where);
	}

	$sql = "
	SELECT DISTINCT
	       tcp.tcpid,
	       tcp.ungcodproponente,
	       unp.gescod AS gescodproponente,
	       rep.cpf AS cpfreplegalproponente,
	       tcp.ungcodconcedente,
	       unc.gescod AS gescodconcedente,
	       rec.cpf AS cpfreplegalconcedente,
	       (select identificacao from ted.justificativa where tcpid = tcp.tcpid) AS tcptitulo,
	       (select justificativa from ted.justificativa where tcpid = tcp.tcpid) AS tcpjustificativa,
	       (select objetivo from ted.justificativa where tcpid = tcp.tcpid) AS tcpobjetivoobjeto,
	       (SELECT htddata
	          FROM workflow.historicodocumento hst
	            JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
	           WHERE hst.docid = doc.docid
	             AND aed.esdiddestino = {$emAnalisePelaCGSO}
	             AND aed.esdidorigem = {$aguardandoAprovacaoSecretario}
	           ORDER BY hst.hstid DESC LIMIT 1) AS data_vigencia,
	       (SELECT
	            sum(provalor) AS total
	        FROM ted.previsaoorcamentaria pro
	            LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pro.ptrid
	            LEFT JOIN monitora.pi_planointerno pi on pi.pliid = pro.pliid
	            LEFT JOIN public.naturezadespesa ndp ON ndp.ndpid = pro.ndpid
	        WHERE
	            pro.tcpid = tcp.tcpid AND
	            ";
		if (isset($_lotid))
			$sql .= " pro.proid IN (select lmi.proid from ted.lotemacroitens lmi where lmi.lotid = {$_lotid})";
		else
			$sql .= " pro.proid IN (".implode(",", $_proid).")";

	      $sql .=      ") AS valor_total,
	       -- Valor utilizado apenas na planilha, onde são incluídos apenas células presentes no lote atual
	       (SELECT total
	          FROM (SELECT SUM(provalor) AS total
	                  FROM ted.previsaoorcamentaria prot
	                    INNER JOIN ted.lotemacroitens loi USING(proid)
	                  WHERE prostatus = 'A'
	                    {$conditionWhere}
	                    AND loi.loistatus = 'A'
	               ) AS foo) AS valor_no_lote,
	       (SELECT DISTINCT crdmesexecucao
	          FROM ted.previsaoorcamentaria promes
	          WHERE tcp.tcpid = promes.tcpid LIMIT 1) AS crdmesexecucao
	  FROM ted.termocompromisso tcp
	    JOIN workflow.documento doc ON doc.docid = tcp.docid
	    LEFT JOIN public.unidadegestora unp ON tcp.ungcodproponente = unp.ungcod
	    LEFT JOIN public.unidadegestora unc ON tcp.ungcodconcedente = unc.ungcod
	    LEFT JOIN ted.representantelegal rep ON (tcp.ungcodproponente = rep.ug AND rep.substituto = 'f')
	    LEFT JOIN ted.representantelegal rec ON (tcp.ungcodconcedente = rec.ug AND rec.substituto = 'f')
	    JOIN ted.previsaoorcamentaria po ON (po.tcpid = tcp.tcpid)
	  {$whereStr}
	  ORDER BY tcp.tcpid";
	//ver($sql);
	return $db->carregar($sql);
}

function makeDateSoma($date, $days=0, $mounths=0, $years=0)
{
	$date = explode("/", $date);
	return date('d/m/Y', mktime(0, 0, 0, $date[1] + $mounths, $date[0] +  $days, $date[2] + $years) );
}

function celulaOrcamentariaTable($tcpid, $loteid = null, $xls = null) {
	global $db;
	//ver($tcpid, $loteid, $xls);
	$dados = getCelulaOrcamentaria($tcpid);
	//ver($dados);

	if (null !== $loteid) {

		$strSQL = "select proid from ted.lotemacroitens where lotid = {$loteid} AND loistatus = 'A'";
		$itensSelected = $db->carregar($strSQL);
		$proIds = array();
		if ($itensSelected) {
			foreach ($itensSelected as $linha) {
				array_push($proIds, $linha['proid']);
			}
		}
	} else {

		$strSQL = "select proid from ted.lotemacroitens where tcpid = {$tcpid} AND loistatus = 'A'";
		$itensSelected = $db->carregar($strSQL);
		$proIds = array();
		if ($itensSelected) {
			foreach ($itensSelected as $linha) {
				array_push($proIds, $linha['proid']);
			}
		}
	}

	if ($dados[0] != '') {

		//retira as celulas orçamentárias que ja foram descentralizadas via lote
		foreach ($dados as $k => $dado) {
			if (is_array($proIds) && in_array($dado['proid'], $proIds)) {
				unset($dados[$k]);
			}

			//Retira também as celulas orçamentárias que já foram descentralizadas
			$existePrev = $db->pegaLinha("select * from ted.previsaoparcela where proid = {$dado['proid']}");
			if ($existePrev) {
				unset($dados[$k]);
			}
		}

		$arAnosPrevisao = array();
		$arLote = array();
		$totalPrevisao = count($dados)-1;

		if (!$xls)
			echo '<div class="celula_container">';

		echo monta_cabecalho_celula_orcamentaria();

		foreach ($dados as $k => $dado) {

			if ($xls && is_array($proIds) && !in_array($dado['proid'], $proIds))
				continue;

			if (!in_array($dado['lote'], $arLote)) {
				if ($subTotalPorLote>0) {
					echo '<tr bgcolor="#f0f0f0" id="tr_'.$k.'">
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td align="right"><img align="absmiddle" src="../imagens/icone_lupa.png" border="0" class="linkLote" codigo="'.$loteAnterior.'" alt="Visualizar lote" title="Visualizar lote"/>&nbsp;<span class="linkLote" codigo="'.$loteAnterior.'"><b>Subtotal ('.($loteAnterior ? $loteAnterior : 'lote não encontrado').')</b></span>&nbsp;</td>
                            <td align="right" id="td_subtotalano_'.($loteAnterior ? $loteAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorLote).'</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                          </tr>';
				}
				array_push($arLote, $dado['lote']);
				$subTotalPorLote = 0;
				$loteAnterior = $dado['lote'];
			}

			?>
            <tr id="tr_<?=$dado['proid']?>">
                <td align="center">
                    <?php
                    if (!$xls) {
                        echo "<input name='proid[]' {$checked} id='checkEnvio_{$dado['proid']}' value='{$dado['tcpid']}-{$dado['proid']}' type='checkbox'/>";
                    }
                    ?>
                </td>

                <td align="center" id="td_anoref_<?=$dado['proid']; //$k ?>" width="14%">
                    <?php if($habilita_Natur == 'S'){?>
                        <?php
                        for($z=0;$z<=10;$z++){
                            $arAnosRef[$z]['codigo']	= 2013+$z;
                            $arAnosRef[$z]['descricao']	= 2013+$z;
                        }

                        $db->monta_combo('proanoreferencia[]',$arAnosRef, 'S', 'Selecione...','',$opc,'','','S', 'proanoreferencia_'.$dado['proid'], '', $dado['proanoreferencia'], $title= null);
                        ?>
                    <?php }else{ ?>
                        <?php echo $dado['proanoreferencia'] ? $dado['proanoreferencia'] : '-'; ?>
                        <input type="hidden" name="proanoreferencia[]" id="proanoreferencia_<?php echo $dado['proid']; ?>" value="<?=$dado['proanoreferencia'] ?>"/>
                    <?php } ?>
                </td>

                <td align="center" id="td_acao_<?=$k ?>">
                    <?=$dado['acacod'] ?>
                </td>

                <?php if($habilita_Plano == 'S'){?>
                    <td align="center" id="td_prg_<?=$dado['proid']; ?>"  width="10%">
                        <?php
                        $sql = "SELECT DISTINCT
                                        p.ptrid as codigo,
                                        ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
                                    FROM monitora.ptres p
                                    JOIN public.unidadegestora u
                                        ON u.unicod = p.unicod
                                    WHERE p.ptrano = '2013'
                                    AND p.ptrstatus = 'A'
                                    AND u.unicod IN ( '26101','26298','26291','26290' )";
                        ?>
                        <?=campo_texto('ptrid_temp[]','S','S','',20,27,'','', 'left', '', '', 'id="ptridtemp_'.$dado['proid'].'"', '', $dado['ptrid_descricao']);?>
                        <input type="hidden" name="ptrid[]" id="ptrid_<?php echo $dado['proid']; ?>" value="<?php echo $dado['ptrid']; ?>" />
                    </td>
                <?php }else{ ?>
                    <td align="center" id="td_prg_<?=$k ?>">
                        <input type="hidden" name="ptrid[]" value="<?=$dado['ptrid']?>">
                        <?=$dado['ptrid_descricao']?$dado['ptrid_descricao']:'-'?>
                    </td>
                <?php }?>

                <?php if ($habilita_Plano == 'S') { ?>
                    <td align="center" id="td_pi_<?=$dado['proid']; //$k ?>"  width="10%">
                        <?php
                        if( $dado['ptrid'] != '' ){
                            $sql = "SELECT p.pliid as codigo,
                                            plicod||' - '||plidsc as descricao
                                    FROM monitora.pi_planointerno p
                                    INNER JOIN monitora.pi_planointernoptres pt on pt.pliid = p.pliid
                                    WHERE pt.ptrid = ".$dado['ptrid']."
                                    ORDER by 2";

                            echo $db->monta_combo('pliid[]',$sql, 'S', 'Selecione...','',$opc,'','85','S', 'pliid', '', $dado['pliid'], $title= null);
                        }
                        ?>
                    </td>
                <?php } else { ?>
                    <td align="center" id="td_pi_<?=$dado['proid']; //$k ?>">
                        <input type="hidden" name="pliid[]" value="<?=$dado['pliid']?>">
                        <?=$dado['pliid_descricao']?$dado['pliid_descricao']:'-'?>
                    </td>
                <?php } ?>

                <td align="center" id="td_acaodsc_<?=$dado['proid']; //$k ?>">
                    <?=$dado['acatitulo']?$dado['acatitulo']:'-'?>
                </td>

                <?php if($habilita_Natur == 'S'){?>
                    <td align="center" width="23%">
                        <?php
                        $sql = "SELECT 	DISTINCT ndpid as codigo,
                                     substr(ndpcod, 1, 6) || ' - ' || ndpdsc as descricao
                                FROM public.naturezadespesa
                                WHERE ndpstatus = 'A' and sbecod = '00' and edpcod != '00' and substr(ndpcod,1,2) not in ( '31', '32', '46', '34' )
                                AND ( substr(ndpcod, 3, 2) in ('80', '90', '91','40') or substr(ndpcod, 1, 6) in ('335041','339147','335039', '445041', '333041') )
                                order by 2";
                        //ver($sql);
                        $db->monta_combo('ndpid[]',$sql,'S','Selecione...','',$opc,'','250','S', 'ndpid', '', $dado['ndpid'], $title= null);
                        ?>
                    </td>
                <?php }else{?>
                    <td align="center">
                        <input type="hidden" name="ndpid[]" value="<?=$dado['ndpid']?>">
                        <?=$dado['ndp_descricao']?$dado['ndp_descricao']:'-'?>
                    </td>
                <?php }?>

                <?php if($habilita_Natur == 'S'){?>
                    <td align="center" width="16%" >
                        <?=campo_texto('provalor[]','S','S','',17,17,'[.###],##','', 'right', '', '', 'id="provalor_'.$dado['proid'].'"', '', $dado['provalor']);?>
                    </td>
                <?php }else{?>
                    <td align="center">
                        <input type="hidden" name="provalor[]" value="<?=$dado['provalor']?>">
                        <?=$dado['provalor']?>
                    </td>
                <?php }?>

                <?php if($habilita_Meslib == 'S'){ ?>
                    <td align="center" width="10%">
                        <?php
                        $sql = Array(Array('codigo'=>1,'descricao'=>'Janeiro'),
                            Array('codigo'=>2,'descricao'=>'Fevereiro'),
                            Array('codigo'=>3,'descricao'=>'Março'),
                            Array('codigo'=>4,'descricao'=>'Abril'),
                            Array('codigo'=>5,'descricao'=>'Maio'),
                            Array('codigo'=>6,'descricao'=>'Junho'),
                            Array('codigo'=>7,'descricao'=>'Julho'),
                            Array('codigo'=>8,'descricao'=>'Agosto'),
                            Array('codigo'=>9,'descricao'=>'Setembro'),
                            Array('codigo'=>10,'descricao'=>'Outubro'),
                            Array('codigo'=>11,'descricao'=>'Novembro'),
                            Array('codigo'=>12,'descricao'=>'Dezembro')
                        );
                        $db->monta_combo('crdmesliberacao[]',$sql,'S','Selecione...','',$opc,'','85','S', 'crdmesliberacao', '', $dado['crdmesliberacao'], $title= null);
                        ?>
                    </td>
                <?php }else{ ?>
                    <td align="center"  width="50%">
                        <input type="hidden" name="crdmesliberacao[]" id="'crdmesliberacao[]" value="<?=$dado['crdmesliberacao']?>">
                        <?php echo $dado['crdmesliberacao'] ? mes_extenso($dado['crdmesliberacao']) : '-';?>
                    </td>
                <?php }?>

                <?php if ($habilita_Parc == 'S') { ?>
                    <td align="center"  width="50%">
                        <?php
                        $sql = array();
                        for($i = 1; $i <= 50; $i++){
                            $sql[$i-1]['codigo'] = $i;
                            $sql[$i-1]['descricao'] = $i.' Mês(s)';
                        }
                        array_push($sql, $sql);
                        $db->monta_combo('crdmesexecucao[]', $sql,'S','Selecione...','',$opc,'','100','S', 'crdmesexecucao', '', $dado['crdmesexecucao'], $title= null);
                        ?>
                    </td>
                <?php }  else{ ?>
                    <td align="center">
                        <input type="hidden" name="crdmesexecucao[]" id="'crdmesexecucao[]" value="<?=$dado['crdmesexecucao']?>">
                        <?php echo $dado['crdmesexecucao'].' Mês(s)' ?>
                    </td>
                <?php }?>
            </tr>
            <?php

            // TODO: realizar a somatória baseada no mês
            if( $dado['lote'] != null )
                $subTotalPorLote = $subTotalPorLote+$dado['valor'];
        }
    }
    echo '</table>';

    if (!$xls) {
        echo '</div>';
    }
}

function monta_cabecalho_celula_orcamentaria() {
	return '
    <table id="previsao" class="table table-striped table-bordered table-hover tabela-listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
        <tr id="tr_titulo">
			<th class="subtitulocentro">&nbsp;</th>
			<th class="subtitulocentro"  width="10%">Ano</th>
			<th class="subtitulocentro" width="">Ação</th>
			<th class="subtitulocentro" width="14%">Programa de Trabalho</th>
			<th class="subtitulocentro" width="10%">Plano Interno</th>
			<th class="subtitulocentro" width="">Descrição da Ação Constante da LOA</th>
			<th class="subtitulocentro" width="20%">Nat.da Despesa</th>
			<th class="subtitulocentro" width="13%">Valor (em R$ 1,00)</th>
			<th class="subtitulocentro" width="8%">Mês da Liberação</th>
			<th class="subtitulocentro" width="20%">Prazo para o cumprimento do objeto</th>
		</tr>';
}

function getCelulaOrcamentaria($tcpid) {
	global $db;
	$sql = <<<DML
SELECT DISTINCT
       pro.proid,
       pro.tcpid,
       ptres || ' - ' || p.funcod || '.' || p.sfucod ||'.' || p.prgcod || '.'
             || p.acacod || '.' || p.unicod || '.' || p.loccod AS ptrid_descricao,
       SUBSTR(pi.plicod || ' - ' || pi.plidsc, 1, 45) ||'...' AS pliid_descricao,
       SUBSTR(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc AS ndp_descricao,
       pro.ptrid,
       a.acacod,
       pro.pliid,
       CASE WHEN a.acatitulo IS NOT NULL THEN substr(a.acatitulo, 1, 70) || '...'
            ELSE SUBSTR(a.acadsc, 1, 70) || '...'
         END AS acatitulo,
       pro.ndpid,
       TO_CHAR(pro.provalor, '999G999G999G999G999D99') AS provalor,
       COALESCE(pro.provalor, 0) AS valor,
       crdmesliberacao,
       crdmesexecucao,
       pro.proid,
       pro.proanoreferencia,
       pro.prodata,
       (SELECT ppa2.codncsiafi
          FROM ted.previsaoparcela ppa2
          WHERE ppa2.ppaid = (SELECT MAX(ppa1.ppaid)
                                FROM ted.previsaoparcela ppa1
                                WHERE ppa1.proid = pro.proid)
            AND ppa2.ppacancelarnc = 'f') AS lote,
       pp.codsigefnc,
       pp.codncsiafi
  FROM ted.previsaoorcamentaria pro
    LEFT JOIN monitora.pi_planointerno pi           ON pi.pliid = pro.pliid
    LEFT JOIN monitora.pi_planointernoptres pts     ON pts.pliid = pi.pliid
    LEFT JOIN public.naturezadespesa ndp            ON ndp.ndpid = pro.ndpid
    LEFT JOIN monitora.ptres p                      ON p.ptrid = pro.ptrid
    LEFT JOIN monitora.acao a                       ON a.acaid = p.acaid

    LEFT JOIN public.unidadegestora u 			    ON u.unicod = p.unicod
	LEFT JOIN monitora.pi_planointernoptres pt 	    ON pt.ptrid = p.ptrid

    LEFT JOIN ted.previsaoparcela pp            ON (pp.proid = pro.proid)

    LEFT JOIN ted.termocompromisso tc           ON (tc.tcpid = pro.tcpid)
	LEFT JOIN public.unidadegestora unc             ON (unc.ungcod = tc.ungcodconcedente)

  WHERE pro.prostatus = 'A'
    AND pro.tcpid = {$tcpid}
  ORDER BY lote,
           pro.proanoreferencia DESC,
           crdmesliberacao
DML;
	//    ver($sql, d);
	$dados = !empty($tcpid) ? $db->carregar($sql) : array();
	return $dados;
}

/**
 * Alinha o texto para a direita
 * @param mixed $valor Valor para ser formatado.
 * @return String
 */
function alinhaParaDireita($valor)
{
	$valor = "<p style=\"text-align: right !important;\">$valor</p>" ;
	return $valor;
}

function retornaColunasELabels() {
	return array(
			'descricao' => 'Descrição',
			'unidadegestorap' => 'UG do Proponente',
			'unidadegestorac' => 'UG do Cedente',
			'resp_proponente' => 'Representante do Proponente',
			'resp_concedente' => 'Representante do Cedente',
			'esddsc' => 'Estado da Documentação',
			'coodsc' => 'Coordenação',
			'valor' => 'Valor da Proposta',
			'ptres_desc' => 'Programa de Trabalho',
			'plicod' => 'Plano Interno',
			'ntddsc' => 'Natureza da Despesa',
	);
}

/**
 * Consulta as notas de crédito para listagem na tela de trâmite ou para envio da NC para o SIGEF.
 * O conjunto do resultado é alterado por <$listagem>.
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param int $tcpid ID do termo de execução descentralizado.
 * @param bool $listagem Indica se os campos de listagem devem ser inclusos na query.
 * @return array|bool Listagem dos campos do XML ou do relatório de listagem.
 * @see abaTramite.inc
 * @see solicitarCadastroDeNotasDeCreditoSIGEF()
 **/
function consultarNotasDeCredito($tcpid, $listagem = false, $filtrar = false)
{
	global $db;

	$selectAdicional = '';
	if ($listagem) {
		$selectAdicional = <<<DML
, COALESCE(ppa.ppaultimoretornosigef, '-') AS ppaultimoretornosigef,
       CASE WHEN ppa.ppacadastradosigef THEN 'Sim' ELSE 'Não' END AS ppacadastradosigef
DML;
	}

	$whereAdicional = '';
	if ($filtrar) {
		$whereAdicional = <<<DML
AND ppa.ppacadastradosigef = false
DML;
	}

	// -- IMPORTANTE
	// -- Não adicionar ou remover ou reordenar os campos do select sem considerar no XML
	// -- de solicitacao da NC em solicitarCadastroDeNotasDeCreditoSIGEF().
	$sql = <<<DML
SELECT
       ppa.proid,
       ungcodemitente AS unidade_gestora_emitente,
       gescodemitente AS gestao_emitente,
       ungcodproponente as unidade_gestora_favorecida,
       ung.gescod AS gestao_favorecida,
       tcp.tcpnumprocessofnde AS processo,
       tcp.tcpprogramafnde AS programa,
       'false' AS estorno,
       -- '' AS numero_convenio,
       -- '' AS ano_convenio,
       (select sum(pp.ppavlrparcela) from ted.previsaoparcela pp where pp.proid in (
            select proid from ted.previsaoorcamentaria t where t.tcpid = %d and t.prostatus = 'A' and pp.ppacadastradosigef = 'f')
       ) AS valor_custeio,
       0 AS valor_capital,
       '' AS complemento,
       pro.proanoreferencia AS exercicio,
       --ppa.tcpnumtransfsiafi AS nota_credito,
       ppa.codsigefnc AS nota_credito,
       ptr.ptres,
       pro.prgfonterecurso AS fonte_recurso,
       ndpcod AS natureza_despesa,
       pli.plicod AS plano_interno,
       pro.proanoreferencia AS ano_parcela,
       row_number() OVER (ORDER BY ppaid) AS numero_parcela,
       pro.crdmesliberacao AS mes_pagamento,
       pro.provalor AS valor_pagamento,
       TO_CHAR(ppadata, 'DD/MM/YYYY') as ppadata
       {$selectAdicional}
  FROM ted.previsaoparcela ppa
    INNER JOIN ted.previsaoorcamentaria pro ON(pro.proid = ppa.proid)
    inner join ted.termocompromisso tcp ON(tcp.tcpid = pro.tcpid)
    INNER JOIN public.unidadegestora ung ON (tcp.ungcodproponente = ung.ungcod)
    INNER JOIN monitora.ptres ptr USING(ptrid)
    INNER JOIN public.naturezadespesa ndp USING(ndpid)
    INNER JOIN monitora.pi_planointerno pli ON(pli.pliid = pro.pliid)
  WHERE tcp.tcpid = %d
  {$whereAdicional}
DML;
  $stmt = sprintf($sql, $tcpid, $tcpid);
  //ver($stmt, d);
  return $db->carregar($stmt);
}

/**
 * Pega todas as notas de crédito ja submetidas para pagamento
 * usando um determinado TCPID
 * @return array|null
 */
function pagamentosSolicitados($tcpid, $listagem = false, $filtrar = false) {
	global $db;

	$selectAdicional = '';
	if ($listagem) {
		$selectAdicional = <<<DML
, COALESCE(ppa.ppaultimoretornosigef, '-') AS ppaultimoretornosigef,
       CASE WHEN ppa.ppacadastradosigef THEN 'Sim' ELSE 'Não' END AS ppacadastradosigef
DML;
	}

	$whereAdicional = '';
	if ($filtrar) {
		$whereAdicional = <<<DML
AND ppa.ppacadastradosigef = true
DML;
	}

	// -- IMPORTANTE
	// -- Não adicionar ou remover ou reordenar os campos do select sem considerar no XML
	// -- de solicitacao da NC em solicitarCadastroDeNotasDeCreditoSIGEF().
	$sql = <<<DML
SELECT
       ppa.proid,
       ungcodemitente AS unidade_gestora_emitente,
       gescodemitente AS gestao_emitente,
       ungcodproponente as unidade_gestora_favorecida,
       ung.gescod AS gestao_favorecida,
       tcp.tcpnumprocessofnde AS processo,
       tcp.tcpprogramafnde AS programa,
       'false' AS estorno,
       -- '' AS numero_convenio,
       -- '' AS ano_convenio,
       (select sum(pp.ppavlrparcela) from ted.previsaoparcela pp where pp.proid in (
            select proid from ted.previsaoorcamentaria t where t.tcpid = %d and t.prostatus = 'A' and pp.ppacadastradosigef = 'f')
       ) AS valor_custeio,
       0 AS valor_capital,
       '' AS complemento,
       pro.proanoreferencia AS exercicio,
       --ppa.tcpnumtransfsiafi AS nota_credito,
       ppa.codsigefnc AS nota_credito,
       ptr.ptres,
       pro.prgfonterecurso AS fonte_recurso,
       ndpcod AS natureza_despesa,
       pli.plicod AS plano_interno,
       pro.proanoreferencia AS ano_parcela,
       row_number() OVER (ORDER BY ppaid) AS numero_parcela,
       pro.crdmesliberacao AS mes_pagamento,
       pro.provalor AS valor_pagamento,
       to_char(ppa.ppadata, 'DD/MM/YYYY') as ppadata
       {$selectAdicional}
  FROM ted.previsaoparcela ppa
    INNER JOIN ted.previsaoorcamentaria pro ON(pro.proid = ppa.proid)
    inner join ted.termocompromisso tcp ON(tcp.tcpid = pro.tcpid)
    INNER JOIN public.unidadegestora ung ON (tcp.ungcodproponente = ung.ungcod)
    INNER JOIN monitora.ptres ptr USING(ptrid)
    INNER JOIN public.naturezadespesa ndp USING(ndpid)
    INNER JOIN monitora.pi_planointerno pli ON(pli.pliid = pro.pliid)
  WHERE tcp.tcpid = %d
  {$whereAdicional}
DML;
  $stmt = sprintf($sql, $tcpid, $tcpid);
  //ver($stmt, d);
  return $db->carregar($stmt);
}

/**
 * Faz o select para Pagamento FNDE, somente as celular orçamentárias indicadas
 * via checkbox
 * @return array
 */
function pegaNCPagamentos($tcpid, array $proid = array()) {
	global $db;

    if (!$tcpid || !count($proid)) return false;

	$queryCusteio = sprintf("
        select sum(po.provalor) from ted.previsaoorcamentaria po
        inner join public.naturezadespesa nd on (po.ndpid = nd.ndpid)
        where tcpid = %d and proid in (".implode(',', $proid).") and substr(nd.ndpcod, 1, 2) = '33'
    ", $tcpid);

	$queryCapital = sprintf("
        select sum(po.provalor) from ted.previsaoorcamentaria po
        inner join public.naturezadespesa nd on (po.ndpid = nd.ndpid)
        where tcpid = %d and proid in (".implode(',', $proid).") and substr(nd.ndpcod, 1, 2) = '44'
    ", $tcpid);

	$strSQL = "
        SELECT
            ungcodemitente AS unidade_gestora_emitente,
            gescodemitente AS gestao_emitente,
            ungcodproponente as unidade_gestora_favorecida,
            ung.gescod AS gestao_favorecida,
            tcp.tcpnumprocessofnde AS processo,
            tcp.tcpprogramafnde AS programa,
            'false' AS estorno,
            -- '' AS numero_convenio,
            -- '' AS ano_convenio,
            (%s) AS valor_custeio,
            coalesce((%s), 0) AS valor_capital,
            '' AS complemento,
            pro.proanoreferencia AS exercicio,
            --ppa.tcpnumtransfsiafi AS nota_credito,
            ppa.codsigefnc AS nota_credito,
            ptr.ptres,
            pro.prgfonterecurso AS fonte_recurso,
            ndpcod AS natureza_despesa,
            pli.plicod AS plano_interno,
            pro.proanoreferencia AS ano_parcela,
            row_number() OVER (ORDER BY ppaid) AS numero_parcela,
            pro.crdmesliberacao AS mes_pagamento,
            pro.provalor AS valor_pagamento,
            COALESCE(ppa.ppaultimoretornosigef, '-') AS ppaultimoretornosigef,
            CASE WHEN ppa.ppacadastradosigef THEN 'Sim' ELSE 'Não' END AS ppacadastradosigef,
            pro.proid
        FROM ted.previsaoparcela ppa
            INNER JOIN ted.previsaoorcamentaria pro USING(proid)
            inner join ted.termocompromisso tcp using(tcpid)
            INNER JOIN public.unidadegestora ung ON(tcp.ungcodproponente = ung.ungcod)
            INNER JOIN monitora.ptres ptr USING(ptrid)
            INNER JOIN public.naturezadespesa ndp USING(ndpid)
            INNER JOIN monitora.pi_planointerno pli ON(pli.pliid = pro.pliid)
        WHERE
            ppa.proid in (%s) AND
            ppa.ppacadastradosigef = false
    ";

	$subStmt = sprintf($strSQL, $queryCusteio, $queryCapital, '%s');
	//ver($subStmt, d);
	$stmt = sprintf($subStmt, implode(',', $proid));
	//ver($stmt, d);
	return $db->carregar($stmt);
}



function solicitarCadastroDeNotasDeCreditoSIGEF($dados) {
	global $db;

    if (!is_array($dados['proid'])) $dados['proid'] = array();

	$listaNC = pegaNCPagamentos($dados['tcpid'], $dados['proid']);
	//ver($listaNC, d);

	if (!$listaNC) {
		return false;
	}

	$proids = array();
	foreach ($listaNC as $i => $v) {

		/**
		 * Solução paleativa para linhas duplicadas
		 */
		if (in_array($v['proid'], $proids)) {
			unset($listaNC[$i]);
		}
		array_push($proids, $v['proid']);

		if (strlen($v['programa']) == 1) {
			$listaNC[$i]['programa'] = str_pad($v['programa'], 2, '0', STR_PAD_LEFT);
		}

		if (!empty($listaNC[$i]['valor_custeio'])) {
			$valorCusteio = explode('.', $listaNC[$i]['valor_custeio']);

			/**
			 * Remove zéro a direita de ponto flutuante
			*/
			if ($valorCusteio[1] > 0) {
				$valorPontoFlutuante = rtrim($valorCusteio[1], '0');
				$value = "{$valorCusteio[0]}.$valorPontoFlutuante";
			} else {
				$value = $valorCusteio[0];
			}

			$listaNC[$i]['valor_custeio'] = $value;
		}
	}

	$xmlSolicitarSIGEF = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
<request>
    <header>
        <app>%s</app>
        <version>%s</version>
        <created>%s</created>
    </header>
    <body>
        <auth>
            <usuario>%s</usuario>
            <senha>%s</senha>
        </auth>
        <params>
            <unidade_gestora_emitente>%s</unidade_gestora_emitente>
            <gestao_emitente>%s</gestao_emitente>
            <unidade_gestora_favorecida>%s</unidade_gestora_favorecida>
            <gestao_favorecida>%s</gestao_favorecida>
            <processo>%s</processo>
            <programa>%s</programa>
            <estorno>%s</estorno>
            <valor_custeio>%s</valor_custeio>
            <valor_capital>%s</valor_capital>
            <complemento>%s</complemento>
%s
            <sistema>3</sistema>
            <id_solicitante></id_solicitante>
       </params>
    </body>
</request>
XML;
	$xmlDetalhamento = <<<PARTIAL_XML
            <detalhamento>
                <ano_exercicio>%s</ano_exercicio>
                <nota_credito>%s</nota_credito>
                <celula_orcamentaria>
                    <ptres>%s</ptres>
                    <fonte_recurso>%s</fonte_recurso>
                    <natureza_despesa>%s</natureza_despesa>
                    <plano_interno>%s</plano_interno>
                </celula_orcamentaria>
                <ano_parcela>%s</ano_parcela>
                <numero_parcela>%s</numero_parcela>
                <mes_pagamento>%s</mes_pagamento>
                <valor_pagamento>%s</valor_pagamento>
            </detalhamento>
PARTIAL_XML;


	// -- Dadas da requisição comuns a todas as NC
	$dadosIdentificacaoHeader = array(
        'app' => 'SIMEC',
        'version' => 1,
        'created' => date('c'),
        'usuario' => $dados['sigefusername'],
        'senha' => $dados['sigefpassword'],
	);

	// -- URL da requisição
	if (IS_PRODUCAO) {
        $urlWS = 'https://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
	} else {
        $urlWS = 'https://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/pf';
	}

	/**
	 * Cliente de WEBSERVICE do FNDE.
	 * @see Fnde_Webservice_Client
	 */
	include_once APPRAIZ . 'includes/classes/Fnde_Webservice_Client.class.inc';

	$dadosDaRequisicao = array_merge(
			$dadosIdentificacaoHeader,
			array_slice($listaNC[0], 0, 10, true)
	);
	$todosDetalhamentos = '';
	foreach ($listaNC as $nc) {
		// -- criando um detalhamento para NC da lista
		$todosDetalhamentos.= vsprintf(
				$xmlDetalhamento,
				array_slice($nc, 10)
		);
		//break;
	}
	$dadosDaRequisicao[] = $todosDetalhamentos;
	$xmlRequisicaoSIGEF .= vsprintf($xmlSolicitarSIGEF, $dadosDaRequisicao);
    //ver(simec_htmlentities($xmlRequisicaoSIGEF), d);

	$paramRequest = array(
        'xml' => $xmlRequisicaoSIGEF,
        'method' => 'solicitar'
	);

    $xmlReturn = Fnde_Webservice_Client::CreateRequest()
        ->setURL($urlWS)
        ->setParams($paramRequest)
        ->execute();

	$xml = simplexml_load_string($xmlReturn);
    //ver($xml);

	if (isset($xml->status->error)) {
		$statusText = $xml->status->message->code.' - '. $xml->status->message->text . "\n" .
				$xml->status->error->message->code .' - '. $xml->status->error->message->text;
	} else {
		$statusText = $xml->status->message->code.' - '. $xml->status->message->text;
	}

	// ofuscando a senha para salvar log do xml de envio no db
	$logXmlSend = new SimpleXMLElement($xmlRequisicaoSIGEF);
	$logXmlSend->body->auth->senha = '************';
	$_xmlRequisicaoSIGEF_SaveDb = str_replace('"', '\"', $logXmlSend->asXML());

	$statusText = str_replace("'", "", $statusText);
	$xmlReturn = str_replace("'", "", $xmlReturn);

	$_ppacadastradosigef = ((int)$xml->status->result == 1) ? 'true' : 'false';

	if ($msg == ''){
		$msg = utf8_decode($xml->status->error->message->text);
	}

	$param = array(
        'tcpid' => $dados['tcpid'],
        'logmsg' => $msg,
        'logurl' => $urlWS,
        'logtipo' => Ted_Model_Log::ENVIA_NOTA_CREDITO_PAGAMENTO_FNDE,
        'logxmlenvio' => $_xmlRequisicaoSIGEF_SaveDb,
        'logdtretorno' => date("d/m/Y H:i:s"),
        'logxmlretorno' => $xmlReturn,
        'logerro' => $_ppacadastradosigef,
        'logmetodo' => 'solicitar'
	);
	$log = new Ted_Model_Log($param);
	$log->salvar();
	$log->commit();

	$updateSQL = "
        UPDATE ted.previsaoparcela p SET
        ppaultimoretornosigef = '$statusText',
        ppacadastradosigef = $_ppacadastradosigef
        WHERE proid IN (".implode(',', $dados['proid']).")
    ";
    //ver($updateSQL, d);
	$db->executar($updateSQL);
	$db->commit();

	$xmlFormated = new SimpleXMLElement($xmlReturn);
	$dom = dom_import_simplexml($xmlFormated)->ownerDocument;
	$dom->formatOutput = true;

    if ($xml->status->result == 1) {
    	return true;
	} else {
		return utf8_decode($xml->status->error->message->code.' - '. $xml->status->error->message->text);
	}
}
/*
 * Função para detalhar o Workflow
 */

function listaDetalhesFluxo($tcpid){

    $sql = "
        SELECT
            aed.aeddscrealizar,
            COALESCE(aeddescregracondicao, '-') as aeddescregracondicao,
            aedid
        FROM
            workflow.acaoestadodoc aed
        WHERE
            esdidorigem =
            (
                SELECT
                    esdid
                FROM
                    workflow.documento
                WHERE
                    docid =
                    (
                        SELECT
                            docid
                        FROM
                            ted.termocompromisso
                        WHERE
                            tcpid ={$tcpid} ) )";
    $listagem = new Simec_Listagem($tipoRelatorio = Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $cabecalho = array(
        'Ação',
        'Pré-requisitos para realizar a ação',
        'Pessoa(s) que pode(em) realizar'
    );
    $listagem->addCallbackDeCampo('aeddscrealizar', 'alinhaParaEsquerda');
    $listagem->addCallbackDeCampo('aeddescregracondicao', 'alinhaParaEsquerda');
    $listagem->addCallbackDeCampo('aedid', 'pessoasParaTramitacao');
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $tabela = $listagem->render();

    $saida = "<div id='dadosComplementaresWorkflow'>";
    $saida .= $tabela;
    $saida .= "</div>";

    return $saida;
}



if (!function_exists('number_format2')) {
    function number_format2($number) {
        if (!$number) {
            return 'R$ 0,00';
        }
        return 'R$ ' . number_format($number, 2, ',', '.');
    }
}

/**
 * Envio dinamico de emails para situações pós tramitação
 * @param $docid
 */
function enviar_email_altera_estado($docid) {
    global $db;

    //Pega Situacao da tramitacao
    $tramite = PegaTramitacao($docid);
    //Pega perfis dos destinatarios
    $perfis = PegaPerfisDestinatarios($tramite['esdiddestino']);
    //Pega unidade proponente e concedente
    $dadosTed = $db->pegaLinha("select tcpid, ungcodproponente, ungcodconcedente from ted.termocompromisso where docid = {$docid}");

    //ver($tramite, $perfis, $dadosTed, d);
    if (!$tramite || !$perfis || !$dadosTed) {
        return true;
    }

    //Se for tramitação para "solicitação de alteração" gerar historico do TED
    if ($tramite['aedid'] == WF_ACAO_SOL_ALTERACAO)
        historico_commit($dadosTed['tcpid']);

    //Verifica de quem é a responsabilidade [proponente, concedente]
    $model = new Ted_Model_Responsabilidade();
    $responsability = $model->getDivisaoPerfis();
    foreach ($perfis as $pflcod) {
        if (in_array($pflcod, $responsability['concedente'])) {
            $unidade = $dadosTed['ungcodconcedente'] = getConcedente($dadosTed['tcpid']);
            break;
        } else {
            $unidade = $dadosTed['ungcodproponente'];
            break;
        }
    }

    $tramite['ted'] = $dadosTed['tcpid'];
    $emails = PegaEmailsDestinatarios($perfis, $unidade, $tramite);
    //ver($tramite, $perfis, $unidade, $emails, d);
    if (!$emails || !$unidade) {
        //envia_email_erro($dadosTed, $tramite, $emails, $perfis, $unidade);
        return true;
    }

    $remetente = array('nome' => "Aviso de Tramitação - Termo de Execução Descentralizada: {$dadosTed['tcpid']}", 'email' => $_SESSION['email_sistema']);
    $assunto = "Termo de Execução Descentralizada: {$dadosTed['tcpid']} - {$tramte['fim']}";
    $conteudo = PegaConteudo($tramite);
    setSessionNotification('mensagem', $conteudo);
    $cc = $cco = '';
    if (!IS_PRODUCAO) {
        $conteudo.= '<p>Situação da tramitação: </p>';
        $conteudo.= '<pre>'.print_r($tramite, true).'</pre>';
        $conteudo.= '<p>Grupo de emails</p>';
        $conteudo.= '<pre>'.print_r($emails, true).'</pre>';
        $emails = array($_SESSION['usuemail']);
        $cc = $_SESSION['email_sistema'];
    }

    foreach ($emails as $destinatario)
        enviar_email($remetente, $destinatario, $assunto, $conteudo, $cc, $cco);

    registra_notificacao_usuario();
    return true;
}

/**
 * Pega situação atual, e a situação futura do documento
 * @param integer $docid
 * @return array|bool
 */
function PegaTramitacao($docid) {
    global $db;

    if (!$docid) return false;

    $strSQLTramite = "
        SELECT
            ae.aedid,
            ed.esddsc AS inicio,
            ae.esdidorigem,
            ae.aeddscrealizada AS fim,
            ae.esdiddestino,
            (SELECT TO_CHAR(htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento WHERE docid = {$docid} ORDER BY hstid DESC LIMIT 1) AS dttramite
        FROM workflow.acaoestadodoc ae
        INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ae.esdidorigem)
        WHERE ae.aedid = (
            SELECT aedid FROM workflow.historicodocumento WHERE docid = {$docid} ORDER BY hstid DESC LIMIT 1
        )
    ";
    $tramite = $db->pegaLinha($strSQLTramite);
    if (!$tramite) return false;

    return $tramite;
}

/**
 * Pega os perfis que receberão e-mail com aviso da tramitação
 * @param $esdiddestino
 * @return array|bool
 */
function PegaPerfisDestinatarios($esdiddestino) {
    global $db;

    if (!$esdiddestino) return false;

    $notIN = 52 . ',' . 60 .','. PERFIL_SUPER_USUARIO;

    $strSQLPerfis = sprintf("
        SELECT pflcod FROM workflow.estadodocumentoperfil WHERE aedid IN (
            SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem IN (
                SELECT esdid FROM workflow.estadodocumento WHERE tpdid = 97 AND esdstatus = 'A' AND esdid = %d
            )
        ) AND pflcod NOT IN (%s) GROUP BY pflcod
    ", $esdiddestino, $notIN);
    //ver($strSQLPerfis, d);
    $perfis = $db->carregar($strSQLPerfis);
    if (!$perfis) return false;

    $pflcodAllowed = array();
    foreach ($perfis as $pflcod) {
        array_push($pflcodAllowed, $pflcod['pflcod']);
    }

    return $pflcodAllowed;
}

/**
 * Pega os usucpf dos destinatários
 * @param $pflcod
 * @param $unidade
 * @return array|bool
 */
function PegaEmailsDestinatarios($pflcod, $unidade, $tramite) {
    global $db;

    if (!$pflcod || !$unidade || !$tramite['ted'] || !$tramite['aedid'])
        return false;

    $strSQLuser = "
        SELECT usucpf FROM ted.usuarioresponsabilidade
        WHERE pflcod in (".implode(',',$pflcod).")
        AND rpustatus = 'A' AND ungcod = '{$unidade}'
    ";

    if ($tramite['aedid'] == AEDID_GABINETE_SECRETARIA_AUTARQUIA_ENVIOU_COORDENACAO) {
        $cooid = $db->pegaUm(sprintf("select cooid from ted.termocompromisso where tcpid = %d", $tramite['ted']));
        if ($cooid) {
            $strSQLuser .= sprintf(" AND cooid = %d ", $cooid);
        }
    }

    //ver($strSQLuser,$tramite, d);
    $users = $db->carregar($strSQLuser);
    if (!$users) return false;

    $emails = array();
    $usucpfs = array();
    foreach ($users as $u) {
        array_push($usucpfs, $u['usucpf']);

        if ($email = $db->pegaUm("select usuemail from seguranca.usuario where usucpf = '{$u['usucpf']}'")) {
            array_push($emails, $email);
        }
    }

    setSessionNotification('users', $usucpfs);
    setSessionNotification('tcpid', $tramite['ted']);
    return (count($emails)) ? $emails : false;
}

/**
 * Template de mensagem padrão de troca de estado
 * @param $tramite
 * @return mixed
 */
function PegaConteudo($tramite) {
    $template = "
        <table border='0' width='100%' cellspacing='2' cellpadding='2'>
            <tbody>
                <tr>
                    <td>
                        <p>O Termo de Execução Descentralizada número: <strong>{#ted}</strong>, foi tramitado do estado <strong>\"{#inicio}\"</strong>, para o estado <strong>\"{#fim}\"</strong>, em: {#data}.</p>
                        <p>Tramitado por: {#usunome} - {#usuemail}</p>
                        <p>Atenciosamente,<br />
                           CGSO/SPO<br />
                           Ministério da Educação<br />
                        </p>
                    </td>
                </tr>
            </tbody>
        </table>
    ";

    extract($tramite);
    $search = array('{#ted}', '{#inicio}', '{#fim}', '{#data}', '{#usunome}', '{#usuemail}');
    $subject = array($ted, $inicio, $fim, $dttramite, $_SESSION['usunome'], $_SESSION['usuemail']);
    return str_replace($search, $subject, $template);
}

function getConcedente($ted) {
    global $db;

    $secretarias = array(
        Ted_Model_Responsabilidade::SECADI,
        Ted_Model_Responsabilidade::SETEC,
        Ted_Model_Responsabilidade::SEB
    );

    $sqlComplement = sprintf("select ungcodconcedente from ted.termocompromisso where tcpid = %d", $ted);
    $rsSec = $db->pegaUm(sprintf("select ungcodpoliticafnde from ted.termocompromisso where tcpid = %d", $ted));
    //ver($rsSec);
    if ($rsSec) {
        if (in_array($rsSec, $secretarias)) {
            $sqlComplement = sprintf("select ungcodpoliticafnde from ted.termocompromisso where tcpid = %d", $ted);
        }
    }

    return (string) $db->pegaUm($sqlComplement);
}

/**
 * Grava a notificação de aviso para o usuário
 * @param array $collection
 * return void(0)
 */
function registra_notificacao_usuario() {
    require_once APPRAIZ . 'spo/autoload.php';

    $collection = $_SESSION['ted_notification_collection'];
    if (count($collection)) return false;

    foreach ($collection['users'] as $usucpf) {
        $params = array();
        $params['sisid'] = $_SESSION['sisid'];
        $params['usucpf'] = $usucpf;
        $params['mensagem'] = $collection['messagem'];
        $params['url'] = 'ted.php?modulo=principal/termoexecucaodescentralizada/tramite&acao=A&ted='.$collection['tcpid'];
        cadastrarAvisoUsuario($params);
    }

    clearSessionNotification();
}

/**
 * Set Um valor para variavel de sessão, usado como registro
 * @param $key
 * @param $value
 */
function setSessionNotification($key, $value) {
    if (!array_key_exists('ted_notification_collection', $_SESSION)) {
        $_SESSION['ted_notification_collection'] = array();
    }

    $_SESSION['ted_notification_collection'][$key] = $value;
}

/**
 * Zera o registro
 */
function clearSessionNotification() {
    unset($_SESSION['ted_notification_collection']);
}

/**
 * para correto funcionamento, atribua os parametros nessa ordem
 * [$dadosTed, $tramite, $emails, $perfis, $unidade]
 * @return bool
 */
function envia_email_erro() {
    $args = func_get_args();
    list($dadosTed, $tramite, $emails, $perfis, $unidade) = $args;

    global $db;

    if (!$unidade) {return true;}

    $abrev = $db->pegaUm(sprintf("select ungabrev from unidadegestora where ungcod = '%s'", $unidade));
    $responsavel = ($dadosTed['ungcodproponente'] == $unidade) ? 'Proponente' : 'Concedente';

    $ulList = '<ul>';
    foreach ($perfis as $p) {
        $pfldsc = $db->pegaUm(sprintf("select pfldsc from seguranca.perfil where pflcod = %d", $p));
        $ulList .= "<li>- {$pfldsc}</li>";
    }
    $ulList .= '</ul>';

    $remetente = array('nome' => "Aviso de Tramitação - TED: {$dadosTed['tcpid']}", 'email' => $_SESSION['email_sistema']);
    $assunto = "Termo de Execução Descentralizada: {$dadosTed['tcpid']} - {$tramte['fim']}";
    $template = "
        <p>Situação do TED: {#ted}</p>
        <p>Tramitado de \"{#esdidInicio}\" para \"{#esdidFim}\", em {#dttramite}.</p>
        <p>Termo em responsábilidade da Unidade Gestora {#responsavel} - {#ungcod} / {#ungabrev}, não existe nenhum usuário cadastrado com perfil:<br />
            {#perfil}
        <br />para a unidade gestora descrita acima.</p>
        <p>Esta é uma mensagem automática.</p>
        <p>Atenciosamente<br />
        DTI/SIMEC<p>
    ";

    $search = array('{#ted}', '{#esdidInicio}', '{#esdidFim}', '{#dttramite}', '{#responsavel}', '{#ungcod}', '{#ungabrev}', '{#perfil}');
    $replace = array($tramite['ted'], $tramite['inicio'], $tramite['fim'], $tramite['dttramite'], $responsavel, trim($unidade), trim($abrev), $ulList);
    $conteudo = str_replace($search, $replace, $template);

    //ver($conteudo, d);
    enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $conteudo, '');
    return true;
}

function pos_acao_execucao_descentralizacao($docid) {
    enviar_email_altera_estado($docid);

    if (primeira_vez_execucao($docid)) {
        altera_data_inicio_vigencia($docid);
    }

    altera_data_final_vigencia($docid);
    return true;
}

function altera_data_final_vigencia($docid) {
    global $db;

    $tcpid = $db->pegaUm("select tcpid from ted.termocompromisso where docid = {$docid}");
    if (!$tcpid) return false;

    $vigenciaSetada = $db->pegaLinha("select * from ted.aditivovigencia where tcpid = {$tcpid} order by vigdata desc limit 1");
    if ($vigenciaSetada) {
        $strSQL = "
            update ted.termocompromisso
                set dtvigenciafinal = '{$vigenciaSetada['vigdata']}'
            where tcpid = {$vigenciaSetada['tcpid']}
        ";

        $db->executar($strSQL);
        return $db->commit();
    }
}

function primeira_vez_execucao($docid) {
    global $db;

    $tcpid = $db->pegaUm("select tcpid from ted.termocompromisso where docid = {$docid}");
    if (!$tcpid) return false;

    $strSQL = "
        select count(*) from workflow.historicodocumento
        where aedid = 1618 and docid = $docid
    ";

    return (boolean) ($db->pegaUm($strSQL) == 1);
}

function altera_data_inicio_vigencia($docid) {
    global $db;

    $tcpid = $db->pegaUm("select tcpid from ted.termocompromisso where docid = {$docid}");
    if (!$tcpid) return false;

    $strSQL = "
        select * from workflow.historicodocumento
        where aedid = 1618
        and docid = $docid
        order by hstid asc limit 1
    ";

    $params = $db->pegaLinha($strSQL);
    $strSQL = "
        update ted.termocompromisso set dtvigenciaincial = '{$params['htddata']}' where tcpid = {$tcpid}
    ";
    $db->executar($strSQL);
    return $db->commit();
}

/**
 * Formata o código do PI para exibir um popover ao passar o mouse por cima.
 *
 * @param string $plicod Código do PI para formatação.
 * @param mixed[] $dados Conjunto de dados da linha.
 */
function formatarOrcamentarioPi($plicod, $dados)
{
    return <<<HTML
<abbr data-toggle="tooltip" data-placement="top" title="{$dados['plidsc']}">{$plicod}</abbr>
HTML;
}

/**
 * Formata o código da natureza para exibir um popover ao passar o mouse por cima.
 *
 * @param string $ndpcod Código da natureza para formatação.
 * @param mixed[] $dados Conjunto de dados da linha.
 */
function formatarOrcamentarioNatureza($ndpcod, $dados)
{
    return <<<HTML
<abbr data-toggle="tooltip" data-placement="top" title="{$dados['ndpdsc']}">{$ndpcod}</abbr>
HTML;
}

/**
 * Formata o código do PTRES para exibir um popover ao passar o mouse por cima.
 *
 * @param string $ptres Código do PTRES para formatação.
 * @param mixed[] $dados Conjunto de dados da linha.
 */
function formatarOrcamentarioPtres($ptres, $dados)
{
    $programatica = "{$dados['funcod']}.{$dados['sfucod']}.{$dados['prgcod']}.{$dados['unicod']}.{$dados['acacod']}.{$dados['loccod']}";

    return <<<HTML
<abbr data-toggle="tooltip" data-placement="top" title="{$programatica}">{$ptres}</abbr>
HTML;
}
