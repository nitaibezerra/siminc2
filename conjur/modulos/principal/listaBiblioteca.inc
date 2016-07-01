<?php
ini_set('memory_limit', '128M');

unset($_SESSION['lvaid']);
unset($_SESSION['lveid']);


if ($_REQUEST['evento'] == 'E') {

    $prcid = (string) $_REQUEST['prcid'];

    $sql_D = "UPDATE conjur.processoconjur SET prcstatus = 'I' where prcid = " . $prcid;
    $db->executar($sql_D);
    $db->commit();

    echo "<script>
		alert('Dados salvos com sucesso.');
		window.location = '?modulo=inicio&acao=C';
	  </script>";
    exit;
}

if ($_REQUEST['req'] == 'ACsidoc') {
    ob_clean();
    $q = strtolower($_GET["q"]);
    if ($q != "") {
        $sql = "SELECT prcnumsidoc FROM conjur.processoconjur where prcnumsidoc like ('%$q%')";
    } else {
        $sql = "SELECT prcnumsidoc FROM conjur.processoconjur";
    }
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    if ($q === '')
        return;

    foreach ($arDados as $key => $value) {
        //if (strpos(strtolower($key), $q) !== false) {
        if (trim($value['prcnumsidoc'])) {
            $prcnumsidoc = trim($value['prcnumsidoc']);
            echo "{$prcnumsidoc}|{$prcnumsidoc}\n";
        }
    }

    die;
}

if ($_REQUEST['req'] == 'ACprcnumeroprocjudicial') {
    ob_clean();
    $sql = "SELECT prcnumeroprocjudicial FROM conjur.processoconjur WHERE prcstatus = 'A'";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prcnumeroprocjudicial']), $q) !== false) {
            $prcnumeroprocjudicial = $value['prcnumeroprocjudicial'];
            echo "{$prcnumeroprocjudicial}|{$prcnumeroprocjudicial}\n";
        }
    }

    die;
}

if ($_REQUEST['req'] == 'ACprcnumeroprocjudantigo') {
    ob_clean();
    $sql = "SELECT prcnumeroprocjudantigo FROM conjur.processoconjur WHERE prcstatus = 'A'";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prcnumeroprocjudantigo']), $q) !== false) {
            $prcnumeroprocjudantigo = $value['prcnumeroprocjudantigo'];
            echo "{$prcnumeroprocjudantigo}|{$prcnumeroprocjudantigo}\n";
        }
    }

    die;
}

if ($_REQUEST['req'] == 'ACprcnomeinteressado') {
    ob_clean();

    $encerrado = ( $_REQUEST['encerrados'] == 'sim' ) ? 'AND doc.esdid = 80' : 'AND doc.esdid <> 80';
    $prioritarios = ( $_REQUEST['prioritarios'] == 'sim' ) ? 'AND prc.prcprioritario = true' : '';

    $sql = "
        SELECT  prc.prcnomeinteressado 
	FROM conjur.processoconjur prc
        JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
        JOIN workflow.documento doc ON doc.docid = esp.docid
        WHERE prc.prcstatus = 'A' {$encerrado} {$prioritarios}
    ";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prcnomeinteressado']), $q) !== false) {
            $prcnomeinteressado = $value['prcnomeinteressado'];
            echo "{$prcnomeinteressado}|{$prcnomeinteressado}\n";
        }
    }
    die;
}

if ($_REQUEST['req'] == 'ACtasdsc') {
    ob_clean();
    $sql = "SELECT tasdsc FROM conjur.tipoassunto WHERE tasstatus = 'A' ORDER BY tasdsc";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['tasdsc']), $q) !== false) {
            $tasdsc = $value['tasdsc'];
            echo "{$tasdsc}|{$tasdsc}\n";
        }
    }
    die;
}

if ($_REQUEST['req'] == 'ACanxdesc') {
    ob_clean();

    $encerrado = ( $_REQUEST['encerrados'] == 'sim' ) ? 'AND doc.esdid = 80' : 'AND doc.esdid <> 80';
    $prioritarios = ( $_REQUEST['prioritarios'] == 'sim' ) ? 'AND prc.prcprioritario = true' : '';

    $sql = "
        SELECT  anx.anxdesc 
	FROM conjur.anexos anx
	JOIN conjur.processoconjur prc ON prc.prcid = anx.prcid
	JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
	JOIN workflow.documento doc ON doc.docid = esp.docid
	WHERE anx.anxstatus = 'A' {$encerrado} {$prioritarios} 
        ORDER BY anx.anxdesc
    ";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['anxdesc']), $q) !== false) {
            $anxdesc = $value['anxdesc'];
            echo "{$anxdesc}|{$anxdesc}\n";
        }
    }
    die;
}

if ($_REQUEST['req'] == 'ACprodsc') {
    ob_clean();
    $sql = "SELECT prodsc FROM conjur.procedencia WHERE unpid = '" . $_REQUEST['unpid'] . "' AND prodtstatus = 'A' ORDER BY	prodsc";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prodsc']), $q) !== false) {
            $prodsc = $value['prodsc'];
            echo "{$prodsc}|{$prodsc}\n";
        }
    }
    die;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';


print '<br/>';

// carregando o menu
montaAbaInicio("conjur.php?modulo=principal/listaBiblioteca&acao=A");

$titulo = "Biblioteca";
monta_titulo($titulo, ' &nbsp;');

?>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="./js/conjur.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script language="javascript" type="text/javascript">

    jQuery.noConflict();

    function submeteFormPesquisa(tipo) {
        document.getElementById("tipopesquisa").value = tipo;
        document.getElementById("formulario").submit();
    }
    function validaForm(button) {
        button.disabled = true;

        var numdoc = document.getElementById('nudnumero').value;
        var numdocano = document.getElementById('nudano');

        document.getElementById('evento').value = 'pesquisa';

        if (numdoc != '' && numdocano.value == '') {
            alert('Ano obrigatório para filtro por numero de documento.');
            numdocano.focus();
            button.disabled = false;
            return false;
        }

        document.getElementById('formulario').submit();
    }
    function alteraCampoCpfCnpj(tipo) {
        var cpf = document.getElementById('cpf');
        var cnpj = document.getElementById('cnpj');

        if (tipo == 'cpf') {
            cnpj.value = "";
            cnpj.style.display = "none";
            cpf.style.display = "inline";
        } else {
            cpf.value = "";
            cpf.style.display = "none";
            cnpj.style.display = "inline";
        }
    }

    function conjurJs_digitoVerificador(campoNumeroProcessoSidoc) {
        conferirDigitoVerificadorModulo11(campoNumeroProcessoSidoc);
    }

    function exibirAdvogado(cooid){
        if (cooid == "<?php echo EST_AGUARDATRIBUICAO ?>") {
            document.getElementById('opc_com_sem_adv').style.display = "";
        } else {
            document.getElementById('opc_com_sem_adv').style.display = "none";
            document.getElementById('rdo_com_adv').checked = false;
            document.getElementById('rdo_sem_adv').checked = false;
        }
    }

    function exibeConsultaLocalizacao(){
        if (jQuery('#consultas_localizacao').css('display') == 'inline')
        {
            jQuery('#consultas_localizacao').hide();
            jQuery('#mais_consulta_localizacao').attr('src', '../imagens/mais.gif');
        }
        else
        {
            jQuery('#consultas_localizacao').show();
            jQuery('#mais_consulta_localizacao').attr('src', '../imagens/menos.gif');
        }
    }

    function consultarLocalizacao(cooid){
        if (cooid == 0)
            jQuery('#cooid').val('');
        else
            jQuery('#cooid').val(cooid);

        jQuery('#formulario').submit();
    }

    jQuery(document).ready(function() {

        var url = "conjur.php?modulo=inicio&acao=C&req=ACprodsc&unpid=" + jQuery('#unpid').val();

        jQuery('#prodsc').removeClass('normal ac_input');
        jQuery('#prodsc').addClass('disabled ac_input');

        jQuery('#prcnumsidoc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACsidoc", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prcnomeinteressado').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprcnomeinteressado&encerrados=" + jQuery('#encerrados').val() + "&prioritarios=" + jQuery('#prioritarios').val(), {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#tasdsc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACtasdsc", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prodsc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprodsc", {
            extraParams:
                    {
                        unpid: function() {
                            return jQuery("#unpid").val();
                        }
                    },
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prcnumeroprocjudicial').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprcnumeroprocjudicial", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prcnumeroprocjudantigo').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprcnumeroprocjudantigo", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#anxdesc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACanxdesc&encerrados=" + jQuery('#encerrados').val() + "&prioritarios=" + jQuery('#prioritarios').val(), {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#unpid').change(function() {
//		url = "conjur.php?modulo=inicio&acao=C&req=ACprodsc&unpid="+jQuery(this).val();
//		alert(url);
            if (jQuery(this).val() != '')
            {
                if (jQuery('#prodsc').hasClass('disabled'))
                {
                    jQuery('#prodsc').removeClass('disabled ac_input');
                    jQuery('#prodsc').addClass('normal ac_input');
                }
            }
            else
            {
                jQuery('#prodsc').removeClass('normal ac_input');
                jQuery('#prodsc').addClass('disabled ac_input');

            }
        });

        jQuery('[name=tprid]').click(function()
        {
            // judicial
            if (jQuery(this).val() == '2')
            {
                jQuery('.tr_judicial').show();
            }
            else
            {
                jQuery('.tr_judicial').hide();
            }
        });

        jQuery('#tpdid').change(function()
        {
            if (jQuery(this).val() != '')
            {
                jQuery('.tr_documento').show();
            }
            else
            {
                jQuery('.tr_documento').hide();
            }
        });

        jQuery('#unpid').change(function()
        {
            if (jQuery(this).val() != '')
            {
                jQuery('.tr_procedencia').show();
            }
            else
            {
                jQuery('.tr_procedencia').hide();
            }
        });

        jQuery('#pesquisa_avancada').click(function()
        {
            if (jQuery('.tr_pesq_avancada').css('display') == 'none')
            {
                jQuery('.tr_pesq_avancada').show();

                <?php if ($_REQUEST['esdid'] == EST_AGUARDATRIBUICAO): ?>
                    jQuery('#opc_com_sem_adv').show();
                <?php endif; ?>

                jQuery(this).html('[-] Menos Opções de Busca');
            }
            else
            {
                jQuery('.tr_pesq_avancada').hide();

                <?php if ($_REQUEST['esdid'] == EST_AGUARDATRIBUICAO): ?>
                    jQuery('#opc_com_sem_adv').hide();
                <?php endif; ?>

                jQuery(this).html('[+] Mais Opções de Busca');
            }
        });

        var arRequest = <?= simec_json_encode($_POST) ?>;

        jQuery('#enviar').click(function() {
            window.open('conjur.php?modulo=principal/popupRelPesquisaInicio&acao=A&' + jQuery.param(arRequest),
                    'modelo',
                    "height=600,width=600,scrollbars=yes,top=50,left=200");
        });

        



        jQuery('#limpar').click(function() {
            jQuery('[type="checkbox"]').each(function() {
                jQuery(this).attr('checked', false);
            });
            jQuery('[type="radio"]').each(function() {
                jQuery(this).attr('checked', false);
            });
            jQuery('[type="text"]').each(function() {
                jQuery(this).val('');
            });
            jQuery('select').each(function() {
                jQuery(this).val('');
            });
            jQuery('#formulario').submit();
        });

        jQuery('[name="vertodos"]').click(function() {
            jQuery('#todos').val('true');
            jQuery('#origem').val('avancado');
            validaForm(jQuery(this));
        });

        jQuery('#visualizar').click(function() {
            jQuery('#origem').val('avancado');
            validaForm(jQuery(this));
        });
    });

    function marcarPorName(name) {
        var check = false;
        jQuery('[name="' + name + '"]').each(function() {
            if (jQuery(this).attr('checked') == false) {
                check = true;
            }
        });
        jQuery('[name="' + name + '"]').each(function() {
            jQuery(this).attr('checked', check)
        });
    }

    function numProcesso(obj) {
        var prcnumsidoc = obj.value;

        if (prcnumsidoc == ''){
            return;
        }


        if (!conferirDigitoVerificadorModulo11SemMensagem(obj)){
            if (prcnumsidoc.length != 9){
                alert('Número inválido');
                obj.focus();
                return false;
            }else{
                var ano = prcnumsidoc.substring(0, 4);
                var hoje = new Date();

                if (ano < 1900 || ano > hoje.getFullYear()){
                    alert('número inválido');
                    obj.focus();
                    return false;
                }else{
                    //é um número emec válido
                    return true;
                }
            }
        }else{
            //é um numero sidoc válido
            return true;
        }
    }
    
    function exclusao(url) {
		var questao = confirm("Deseja realmente excluir este registro?")
		if (questao){
			window.location = url;
		}
	}

	function emprestar(lvaid) {
		windowOpen('?modulo=principal/emprestimoLivros&acao=A&lvaid='+lvaid,'blank','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}
	
	function recibo(lvaid) {
		windowOpen('?modulo=principal/reciboLivro&acao=A&lvaid='+lvaid,'blank','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}
	
</script>
<form name="formulario" id="formulario" method="post">

    <table align="center" border="0" cellpadding="0" cellspacing="0" class="tabela">
        <tr>
            <td style="background-color: #e9e9e9; color: #404040; width: 600px;">

                <table class="tabela" align="center" border="0" cellpadding="5"
                       cellspacing="1" style="width: 500px;">
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Patrimônio:</td>
                        <td>
                            <?
                                $lvapatrimonio = $_REQUEST['lvapatrimonio'];
                                echo campo_texto('lvapatrimonio', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="lvapatrimonio"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Título:</td>
                        <td>
                            <?
                                $lvatitulo = $_REQUEST['lvatitulo'];
                                echo campo_texto('lvatitulo', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="lvatitulo"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Autor:</td>
                        <td>
                            <?
                                $lvaautor = $_REQUEST['lvaautor'];
                                echo campo_texto('lvaautor', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="lvaautor"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Editora:</td>
                        <td>
                            <?
                                $anxdesc = $_REQUEST['lvaeditora'];
                                echo campo_texto('lvaeditora', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="lvaeditora"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2" align="center">
                            <input type="button" id="limpar" value="Limpar" style="cursor: pointer;float:right;" />
                            <input type="submit" id="pesquisar" name="pesquisar" value="Pesquisar" style="cursor: pointer;float:right;" > 
                        </td>
                    </tr>
                </table>
            </td>
            <td style="background-color: #e9e9e9; color: #404040;" valign="top">
                <input type="hidden" name="esdid" id="esdid" value="<?= $_REQUEST['esdid'] ?>" />
                <div style="float: center; position: relative; width: 300px;">
                    <fieldset style="background-color: #ffffff;">
                        <legend> 
                            <img id="mais_consulta_situacao" style="cursor: pointer;" onclick="exibeConsultaSituacao();" src="../imagens/menos.gif" /> 
                            <b>Consultas por Situação</b> - ( <a style="cursor:pointer;font-weight:bold;" onclick="marcarPorName('esdid[]');" title="Listar todas as situações">marcar todas</a> ) 
                        </legend>
                        <table id="consultas_situacao" width="100%" style="display: inline;">
                            <?php
                                $sqlSituacaoAndamento = "
                                    SELECT  esdid as codigo, 
                                            esddsc as descricao 
                                    FROM workflow.estadodocumento 
                                    WHERE tpdid='" . WORKFLOW_BIBLIOTECA . "' 
                                    ORDER  BY esddsc
                                ";
                                $dadosSituacao = $db->carregar($sqlSituacaoAndamento);

                                //$situacao = $_REQUEST['todos'] != '' || $_REQUEST['evento'] == '' ? Array(1055,1056) : ( is_array($_REQUEST['esdid']) ? $_REQUEST['esdid'] : Array($_REQUEST['esdid']) );
                                $situacao = ( is_array($_REQUEST['esdid']) ? $_REQUEST['esdid'] : Array($_REQUEST['esdid']) );
                                for ($i = 0; $i < count($dadosSituacao); $i++):
                            ?>
                            <?php if ($i % 2 == 0): ?>
                                <tr>
                                    <td>
                                        <input type="checkbox" <?= ((!in_array($dadosSituacao[$i]['codigo'], $situacao)) && count($situacao) > 0 ? "" : "checked") ?> value="<?= $dadosSituacao[$i]['codigo'] ?>" name="esdid[]">
                                    </td>
                                    <td valign="bottom">
                                        <?= $dadosSituacao[$i]['descricao'] ?>
                                    </td>
                                        <?php if (!is_null($dadosSituacao[$i + 1]['descricao'])): ?>
                                    <td>
                                        <input type="checkbox" <?= ((!in_array($dadosSituacao[$i + 1]['codigo'], $situacao)) && count($situacao) > 0 ? "" : "checked") ?> value="<?= $dadosSituacao[$i + 1]['codigo'] ?>" name="esdid[]">
                                    </td>
                                    <td valign="bottom">
                                        <?= $dadosSituacao[$i + 1]['descricao'] ?>
                                    </td>
                            <?php else: ?>
                                <td></td>
                                <td valign="bottom"></td>
                            <?php endif; ?>
                                </tr>
                            <?php endif; ?>
                            <?php endfor; ?>
                        </table>
                    </fieldset>
                </div>
            </td>
        </tr>
    </table>
    <br>
    <table class="tabela" align="center" border="0" cellpadding="5"
           cellspacing="1">
        <tr>
            <td bgcolor="#CCCCCC" align="left">&nbsp; 
            <? //if ($permissoes['gravar']) { ?>
                    <input type="button" name="cadastrarLivro" value="Cadastrar Livro" style="cursor: pointer;" onclick="this.disabled = true; window.location.href = '?modulo=principal/cadastrarLivros&acao=A';">
            <? //} ?>
            </td>
        </tr>
    </table>
</form>

<br>

<div id="div_lista">

	<?php
	
	if (is_array($_REQUEST['esdid'])) {
    	$filtro .= " AND ed.esdid in (" . implode(',', $_REQUEST['esdid']) . ") ";
    }
 	if ($_REQUEST['lvapatrimonio']) {
    	$filtro .= " AND upper(a.lvapatrimonio) ilike '%".str_to_upper($_REQUEST['lvapatrimonio'])."%' ";
    }
    if ($_REQUEST['lvatitulo']) {
    	$filtro .= " AND upper(a.lvatitulo) ilike '%".str_to_upper($_REQUEST['lvatitulo'])."%' ";
    }
    if ($_REQUEST['lvaautor']) {
    	$filtro .= " AND upper(a.lvaautor) ilike '%".str_to_upper($_REQUEST['lvaautor'])."%' ";
    }
   	if ($_REQUEST['lvaeditora']) {
    	$filtro .= " AND upper(a.lvaeditora) ilike '%".str_to_upper($_REQUEST['lvaeditora'])."%' ";
    }
    
    $botoesAE = "''";
	if( possuiPerfil( Array(PRF_SUPERUSUARIO,PRF_ADMINISTRADOR,PRF_APOIO_BIBLIOTECA) ) ){
		$botoesAE = "'<a href=\"conjur.php?modulo=principal/cadastrarLivros&acao=A&lvaid=' || a.lvaid || '&op3=update\">
					   <img border=0 src=\"../imagens/alterar.gif\" title=\"Alterar\">
					 </a>
					 &nbsp;
					 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'conjur.php?modulo=principal/cadastrarLivros&acao=A&lvaid=' || a.lvaid || '&op3=delete\');\" >
					   <img border=0 src=\"../imagens/excluir.gif\" title=\"Excluir\">
					 </a>&nbsp;'";
	}
	
	$botoesC = "''";
	if( possuiPerfil( Array(PRF_SUPERUSUARIO,PRF_ADMINISTRADOR,PRF_ADVOGADO) ) ){
		$botoesC = "'<a href=\"conjur.php?modulo=principal/consultarLivros&acao=A&lvaid=' || a.lvaid || '\">
					   <img border=0 src=\"../imagens/consultar.gif\" title=\"Consultar\">
					 </a>
					 &nbsp;'";
	}
	
	    
	$sql = "SELECT
				case when ed.esdid in(1074) then
						$botoesAE
					else
						'' 
				end
				||
				$botoesC
				||
				case when ed.esdid in(1076) then
					'<a style=\"cursor:pointer\"  onclick=\"return recibo(\'' || a.lvaid || '\');\" >
					   <img border=0 src=\"../imagens/page_attach.png\" title=\"Recibo\">
					 </a>'
					else
						'' 
				end as acao, 
				a.lvapatrimonio,
				a.lvatitulo,
				a.lvaautor,
				a.lvaeditora,
				a.lvaedicao,
				ed.esddsc as situacao
			FROM 
				conjur.livroacervo a
			LEFT JOIN (SELECT max(lveid) as lveid, lvaid 
					   from conjur.livroemprestimo group by lvaid)
					   e ON e.lvaid = a.lvaid
			LEFT JOIN workflow.documento doc ON doc.docid = a.docid
			LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
			WHERE
				a.lvastatus = 'A'
				{$filtro}
		  	ORDER BY 
				 a.lvatitulo";
	
	$cabecalho = array( "Ação","Patrimônio","Título","Autor","Editora","Edição","Situação");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	?>

	
</div>