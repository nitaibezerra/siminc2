<?php
/**
 * Detalhamento do PI com histórico.
 * $Id: popuphistoricoplanointernoUN.inc 77457 2014-03-18 23:27:48Z mariluciaqueiroz $
 */
/**
 */
require_once APPRAIZ . "planacomorc/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "planacomorc/classes/Pi_PlanoInternoHistorico.class.inc";

#var_dump($_GET); die;
if ($_REQUEST['requisicao'] == 'vincular') {
    extract($_GET);
    $retorno = false;

    ver($_GET);


    $obPi_PlanoInterno = new Pi_PlanoInterno($pliid);
    $obPi_PlanoInterno->plisituacao = $situacao;
    $obPi_PlanoInterno->salvar();
    $sql = "SELECT plicod FROM monitora.pi_planointernohistorico WHERE pliid = $pliid ORDER BY pihdata DESC LIMIT 1";
    if (!$plicodOrigem = $db->pegaUm($sql)) {
        $plicodOrigem = $obPi_PlanoInterno->plicod;
    }

    $obPi_PlanoInternoHistorico = new Pi_PlanoInternoHistorico ();
    $obPi_PlanoInternoHistorico->pliid = $pliid;
    $obPi_PlanoInternoHistorico->usucpf = $_SESSION ['usucpf'];
    $obPi_PlanoInternoHistorico->pihsituacao = $situacao;
    $obPi_PlanoInternoHistorico->plicod = $obPi_PlanoInterno->plicod;
    $obPi_PlanoInternoHistorico->plicodorigem = $plicodOrigem;
    if ($pihobs) {
        $obPi_PlanoInternoHistorico->pihobs = trim(utf8_decode($pihobs));
    }
    $obPi_PlanoInternoHistorico->salvar();

    if ($obPi_PlanoInternoHistorico->commit()) {
        //enviaEmailStatusPi($pliid);
        // die;
        $retorno = true;
    }
    unset($obPi_PlanoInterno);
    unset($obPi_PlanoInternoHistorico);

    echo 'true';
    die();
}

extract($_GET);

$obPi_PlanoInterno = new Pi_PlanoInterno($pliid);

if ($obPi_PlanoInterno->pliid) {
    // $obPi_PlanoInterno->pliid =1030;
    $sql = "SELECT
				pl.pliid,
				ptr.ptres,
				pt.ptrid,
				pt.pipvalor,
				ptr.acaid,
				trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
				sum(ptr.ptrdotacao) as dotacaoinicial,
				round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
                                round(sum( coalesce(ppe.total,0) ),2) as empenhado
			FROM monitora.pi_planointerno pl
			INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid
			LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid
			LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
			LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
			LEFT JOIN siafi.pliptrempenho ppe ON ppe.ptres = ptr.ptres AND pl.plicod = ppe.plicod AND ppe.exercicio = '{$_SESSION['exercicio']}'
			WHERE
					pl.pliid = '" . $obPi_PlanoInterno->pliid . "' AND
					pl.plistatus='A'
	    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
	    	ORDER BY ptr.ptres";
    //ver($sql);
    $acoespl = $db->carregar($sql);
}
$acoespl = ($acoespl) ? $acoespl : array();
?>
<style type="text/css">
    button{cursor:pointer}
</style>
<br>
<form name="formulario" id="formulario" method="post">
    <table bgcolor="#f5f5f5" align="center" class="tabela">
        <tr>
            <td class="subtitulodireita" colspan="2">
        <center>
            <h3>Dados do plano interno</h3>
        </center>
        </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Enquadramento da Despesa:</td>
            <td><?php
                if ($obPi_PlanoInterno->eqdid) {
                    echo $db->pegaUm("SELECT eqdcod || ' - ' || eqddsc FROM monitora.pi_enquadramentodespesa WHERE eqdid='" . $obPi_PlanoInterno->eqdid . "'");
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Subação:</td>
            <td><?php
                if ($obPi_PlanoInterno->sbaid) {
                    echo $db->pegaUm("SELECT sbacod || ' - ' || sbatitulo FROM monitora.pi_subacao WHERE sbaid='" . $obPi_PlanoInterno->sbaid . "'");
                }
                ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Nível/Etapa de Ensino:</td>
            <td><?php
                echo $db->pegaUm("SELECT neecod || ' - ' || needsc FROM monitora.pi_niveletapaensino WHERE neeid='" . $obPi_PlanoInterno->neeid . "'");
                ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Categoria de Apropriação:</td>
            <td><?php echo $db->pegaUm("SELECT capcod || ' - ' || capdsc FROM monitora.pi_categoriaapropriacao WHERE capid='" . $obPi_PlanoInterno->capid . "'"); ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Codifição da Unidade(livre):</td>
            <td><?php echo $obPi_PlanoInterno->plilivre; ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Modalidade de Ensino/Tema/Público:</td>
            <td><?php
                if ($obPi_PlanoInterno->mdeid) {
                    echo $db->pegaUm("SELECT mdecod || ' - ' || mdedsc FROM monitora.pi_modalidadeensino WHERE mdeid='" . $obPi_PlanoInterno->mdeid . "'");
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Título:</td>
            <td><?php echo $obPi_PlanoInterno->plititulo; ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Código do PI:</td>
            <td><?php echo $obPi_PlanoInterno->plicod; ?></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Descrição do PI:</td>
            <td><?php echo $obPi_PlanoInterno->plidsc; ?></td>
        </tr>
        </tr>
        <tr>
            <td class="subtitulodireita">Unidade Orçamentária:</td>
            <td><?php
                $ug = $db->pegaUm("SELECT  unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '" . $obPi_PlanoInterno->unicod . "'");
                if ($ug == null || $ug == '') {
                    $ugS = $db->pegaUm("SELECT  unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = (SELECT unicod FROM public.unidadegestora WHERE ungcod = '" . $obPi_PlanoInterno->ungcod . "')");
                    if ($ugS == null || $ugS == '') {
                        $ugS = " - ";
                        echo $ugS;
                    }
                } else {
                    echo $ug;
                }
                ?></td>
        </tr>
        </tr>
        <tr>
            <td class="subtitulodireita">Unidade Gestora:</td>
            <td><?php
                $ug = $db->pegaUm("SELECT ungcod || ' - ' || ungdsc FROM public.unidadegestora WHERE ungcod = '" . $obPi_PlanoInterno->ungcod . "'");
                if ($ug == null || $ug == '') {
                    $ug = " - ";
                }
                echo $ug;
                ?></td>
        </tr>
        <tr>
            <td colspan="2">
                <?php
                $sql = <<<DML
SELECT TO_CHAR(pih.pihdata, 'DD/MM/YYYY HH24:MI:SS') AS pihdata,
       '<center>' || CASE WHEN pih.pihsituacao = 'P' THEN ' Pendente '
                          WHEN pih.pihsituacao = 'A' THEN ' Aprovado '
                          WHEN pih.pihsituacao = 'R' THEN ' Revisado '
                          WHEN pih.pihsituacao = 'C' THEN ' Cadastrado no SIAFI '
                          WHEN pih.pihsituacao = 'E' THEN ' Enviado para Revisão '
                          WHEN pih.pihsituacao = 'H' THEN ' Homologado '
                       END || '</center>' AS situacao,
       COALESCE(pih.pihobs, '<center>-</center>') AS pihobs,
       u.usunome
  FROM monitora.pi_planointernohistorico pih
    INNER JOIN seguranca.usuario u ON pih.usucpf = u.usucpf
  WHERE pliid = {$obPi_PlanoInterno->pliid}
  ORDER BY pih.pihdata DESC
DML;

                $sql2 = "SELECT plisituacao FROM monitora.pi_planointerno WHERE pliid = $obPi_PlanoInterno->pliid ";
                $ultimaSituacao = $db->pegaUm($sql2);

                $cabecalho = array(
                    "Data Hora",
                    "Situação",
                    "Observação",
                    "Cadastrado Por"
                );
                $db->monta_lista_simples($sql, $cabecalho, 100, 30, 'N', '100%', 'N');
                ?>
            </td>
        </tr>
        <tr bgcolor="#C0C0C0">
            <?
            ?>
            <td align="left" colspan="2">
                <?php
                /* Não exibe nenum botão para o perfil Gabinete */
                $perfis = pegaPerfilGeral();
                if (!in_array(1063, $perfis)) {
                    $sql = <<<DML
                    SELECT pli.unicod
                      FROM monitora.pi_planointerno pli
                      WHERE pli.pliid = {$obPi_PlanoInterno->pliid}
DML;

                    $arUOdoPI = $db->carregar($sql);
                    $arUOdoPI = ($arUOdoPI) ? $arUOdoPI : array();

                    $unicodsPI = array();
                    foreach ($arUOdoPI as $ptres) {
                        array_push($unicodsPI, $ptres ['unicod']);
                    }

                    $arUOdoPTRES = array();
                    if (in_array('26101', $unicodsPI)) {
                        $sql = <<<DML
                    SELECT unicod
                      FROM monitora.pi_planointernoptres pip
                        INNER JOIN monitora.ptres p ON pip.ptrid = p.ptrid
                      WHERE pip.pliid = {$obPi_PlanoInterno->pliid}
DML;
                        $arPtres = $db->carregar($sql);
                        $arPtres = ($arPtres) ? $arPtres : array();
                        foreach ($arPtres as $ptres) {
                            array_push($arUOdoPTRES, $ptres ['unicod']);
                        }
                    }

                    if (!$_GET ['sl']) {
                        $perfisUsuario = pegaPerfilGeral($_SESSION ['usucpf'], $_REQUEST ['sisid']);
                        $pflcod = 1044; // -- www/planacomorc/_constantes.php::PFL_GESTAO_ORCAMENTARIA
                        $sql = <<<DML
    SELECT uni.unicod AS codigo
      FROM planacomorc.usuarioresponsabilidade ur
        INNER JOIN public.unidade uni USING(unicod)
      WHERE ur.usucpf = '{$_SESSION['usucpf']}'
        AND ur.pflcod = '{$pflcod}'
        AND ur.rpustatus = 'A'
DML;
                        $usuarioresponsabilidade = $db->carregar($sql);
                        $usuarioresponsabilidade = $usuarioresponsabilidade ? $usuarioresponsabilidade : array();
                        foreach ($usuarioresponsabilidade as &$ur) {
                            $ur = $ur ['codigo'];
                        }
                        switch ($ultimaSituacao) {
                            case 'P' : // -- Pendente
                                botaoEnviarRevisao();
                                botaoAprovar($unicodsPI, $perfisUsuario, $usuarioresponsabilidade, $pflcod, $arUOdoPTRES);
                                break;
                            case 'H' : // -- Homologação
                                botaoEnviarRevisao();
                                botaoAprovar($unicodsPI, $perfisUsuario, $usuarioresponsabilidade, $pflcod, $arUOdoPTRES);
                                break;
                            case 'E' : // -- Enviado para Revisão
                                botaoTornarPendente();
                                break;
                            case 'A' : // -- Aprovado
                                botaoCadastrarSIAFI($pliid);
                                break;
                            case 'T' : // -- Atualizar no SIAFI - Setado pelo sistema
                                botaoAtualizarSIAFI($pliid);
                                break;
                            case 'C' ://Cadastrado no SIAFI -- adicionado conforme demanda Cod 240042
                                botaoEnviarRevisao();
                                break;
                        }
                    }
                }
                ?>

            </td>

        </tr>
        <tr>
            <td align="center" colspan="2">
                <?php echo $unidade = $db->pegaUm("SELECT unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '" . $_SESSION['monitora_var']['unicod'] . "'"); ?>
            </td>
        </tr>
        <tr>
            <td colspan="2">
                <table cellpadding="0" border="0" width="98%" align="center"
                       id="orcamento"
                       style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;"
                       onmouseover="tabindexcampo();">
                    <tr>
                        <td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento
                                Orçamentário</b></td>
                    </tr>
                    <tr>
                        <td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input
                                type="hidden" name="pliptres"></td>
                        <td style="background-color: #C9C9C9; width: 45%;" align="center"
                            nowrap><b>Ação</b></td>
                        <td style="background-color: #C9C9C9; width: 100px;"
                            align="center" nowrap><b>Dotação do PTRES</b></td>
                        <td style="background-color: #C9C9C9; width: 100px;"
                            align="center" nowrap><b>Dotação da Subação</b></td>
                        <td style="background-color: #C9C9C9; width: 100px;"
                            align="center" nowrap><b>Empenhado no PI</b></td>
                    </tr>
                    <?
                    if ($acoespl [0]) {
                        $valortotalpi = 0;
                        $cor = 0;
                        foreach ($acoespl as $acpl) {
                            ?>
                            <tr style="height:30px;<?php echo (($cor % 2) ? "" : "background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['ptres']; ?>">
                                <td align="center"><?php echo $acpl['ptres']; ?></td>
                                <td align="left"><?php echo $acpl['descricao']; ?></td>
                                <td align="right"><?php echo number_format($acpl['dotacaoinicial'], 2, ',', '.'); ?></td>
                                <td align="right"><?php echo number_format($acpl['dotacaosubacao'], 2, ',', '.'); ?></td>
                                <td align="right"><?php echo number_format($acpl['empenhado'], 2, ',', '.'); ?></td>
                            </tr>
                            <?
                            $cor ++;
                            $valortotalpi = $valortotalpi + $acpl ['pipvalor'];
                        }
                    }
                    ?>
                </table>
            </td>
        </tr>
    </table>
</form>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>




//coloca tabindex no campo valor
                           function tabindexcampo() {
                               var x = document.getElementsByTagName("input");
                               var y = 1;
                               for (i = 0; i < x.length; i++) {
                                   if (x[i].type == "text") {
                                       if (x[i].name.substr(0, 8) == 'plivalor') {
                                           x[i].tabIndex = y;
                                           y++;
                                       }
                                   }
                               }
                           }


//messageObj = new DHTML_modalMessage();	// We only create one object of this class
//messageObj.setShadowOffset(5);	// Large shadow

                           /*** INICIO SHOW MODAL ***/
                           var countModal = 1;
                           function montaShowModal() {
                               var campoTextArea = '<form id="form" name="form"><div class="notprint">' +
                                       '<textarea class="txareaclsMouseOver" id="pihobs' + countModal + '" name="pihobs' + countModal + '" cols="80" rows="8" title="Mensagem" ' +
                                       'onmouseover="MouseOver( this );" ' +
                                       'onfocus="MouseClick( this );" ' +
                                       'onmouseout="MouseOut( this );" ' +
                                       'onblur="MouseBlur( this ); ' +
                                       'textCounter( this.form.pihobs' + countModal + ', this.form.no_pihobs, 500);" ' +
                                       'style="width:75ex;" ' +
                                       'onkeydown="textCounter( this.form.pihobs' + countModal + ', this.form.no_pihobs, 500 );" ' +
                                       'onkeyup="textCounter( this.form.pihobs' + countModal + ', this.form.no_pihobs, 500);">' +
                                       '</textarea><br> ' +
                                       '<input readonly="readonly" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" ' +
                                       'name="no_pihobs" size="6" maxlength="6" value="500" ' +
                                       'class="CampoEstilo" type="text"> ' +
                                       '<font size="1" color="red" face="Verdana"> máximo de caracteres</font> ' +
                                       '</div><div id="print_pihobs" class="notscreen" style="text-align: left;"></div>' +
                                       //checkBoxEmail+
                                       '</form>';
                               var alertaDisplay = '<center><div class="titulo_box" >É necessário colocar observação.<br/ >' + campoTextArea + '</div>'
                                       + '<div class="links_box" ><br><input type="button" onclick="trocarSituacao(\'E\',1)" value="Gravar" />'
                                       + '<input type="button" onclick=\'closeMessage(); return false \' value="Cancelar" /></center>';
                               displayStaticMessage(alertaDisplay, false);
                               return false;
                           }

                           function displayStaticMessage(messageContent, cssClass) {
                               messageObj = new DHTML_modalMessage(); // We only create one object of this class
                               messageObj.setShadowOffset(5); // Large shadow

                               messageObj.setHtmlContent(messageContent);
                               messageObj.setSize(420, 215);
                               messageObj.setCssClassMessageBox(cssClass);
                               messageObj.setSource(false); // no html source since we want to use a static message here.
                               messageObj.setShadowDivVisible(false); // Disable shadow for these boxes
                               messageObj.display();
                           }

                           function closeMessage() {
                               messageObj.close();
                           }

                           function trocarSituacao(situacao) {
                               var pliid = '<?php echo $obPi_PlanoInterno->pliid; ?>';
                               $.ajax({
                                   url: 'planacomorc.php?modulo=principal/planointerno/popuphistoricoplanointerno&acao=A&requisicao=vincular&pliid=' + pliid + '&situacao=' + situacao,
                                   success: function(response) {
                                       alert('Registro alterado com Sucesso.');
                                   }
                               });
                           }
</script>
<div id="dibDebug"></div>

<?php

function botaoEnviarRevisao() {
    ?>
    <input type="button" value="Enviar para Revisão" onclick="montaShowModal()" />
    <input type="button" value="Atualizado no SIAFI" onclick="trocarSituacao('C')" style="color: white" />
    <?php
}

function botaoTornarPendente() {
    ?>
    <input type="button" value="Tornar Pendente" onclick="trocarSituacao('P')" />
    <?php
}

/**
 * Exibe o botão de aprovar de acordo com as permissões do usuário.
 *
 * @param array $unicodPI
 *        	UO de quem solicitou o PI
 * @param array $perfis
 * @param array $unicodsResponsabilidade
 * @param type $pflCode
 * @param type $arUOdoPTRES
 */
function botaoAprovar(array $unicodPI, array $perfis, array $unicodsResponsabilidade, $pflCode, $arUOdoPTRES) {
    $podeAprovar = false;
    if (1 == $_SESSION ['superuser']) {
        $podeAprovar = true;
    } elseif (in_array($pflCode, $perfis)) { // -- Verifica se é o perfil que pode aprovar PIs
        if (1 == count($unicodPI) && in_array(array_pop($unicodPI), $unicodsResponsabilidade)) {
            // -- Se a ÚNICA UO do PI estiver dentro do conjunto UO-RESPONSABILIDADE, deixa aprovar o PI
            $podeAprovar = true;
        }
    }

    if ($podeAprovar) {
        ?>
        <input type="button" value="Aprovar" onclick="trocarSituacao('A')" />
        <?php
    }
}

function botaoCadastrarSIAFI($pliid) {
    ?>
    <input type="button" value="Cadastrar no SIAFI"  onclick="trocarSituacao('C')" />
    <?php
}

function botaoAtualizarSIAFI($pliid) {
    ?>
    <input type="button" value="Cadastrar no SIAFI" onclick="trocarSituacao('T')" style="color: red" />
    <input type="button" value="Atualizado no SIAFI" onclick="trocarSituacao('C')" style="color: white" />
    <?php
}
?>