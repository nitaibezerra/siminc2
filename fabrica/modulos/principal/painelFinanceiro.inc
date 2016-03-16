<?php
$clsFuncoes = new funcoesProjetoFabrica();

//Ajax
if ($_REQUEST['esdidAjax']) {
    header('content-type: text/html; charset=ISO-8859-1');
    $clsFuncoes->exibirFinanceiroSubListaSS($tpeid = 1, $tpEstimadaDetalhada = $_REQUEST['tpED'], $esdid = $_REQUEST['esdidAjax'], $ctrid= $_REQUEST['ctrid'] );
    exit;
}
//problema svn

if (($_REQUEST['esdidOSAjax'])AND (!$_REQUEST['SSOS'])AND (!$_REQUEST['SSOSCANCELADAS'])) {
	header('content-type: text/html; charset=ISO-8859-1');

	if($_REQUEST['tipoLista'] == '21' || $_REQUEST['tipoLista'] == '22') $tpeid = 2;
	//else $tpeid = 1;
	$clsFuncoes->exibirFinanceiroSubListaOS($tpeid, $tpEstimadaDetalhada = $_REQUEST['tpED'], $esdid = $_REQUEST['esdidOSAjax'],$ctrid= $_REQUEST['ctrid']);
  	exit;
}

if (($_REQUEST['SSOS']) AND (!$_REQUEST['SSOSCANCELADAS'])) {
	header('content-type: text/html; charset=ISO-8859-1');
        $tpeid = 1;

	$clsFuncoes->exibirFinanceiroSubListaSSOS($tpeid, $tpEstimadaDetalhada = $_REQUEST['tpED'], $esdid = $_REQUEST['esdidOSAjax'],$ctrid= $_REQUEST['ctrid']);
  	exit;
}

if  (($_REQUEST['SSOSCANCELADAS'])) {
	header('content-type: text/html; charset=ISO-8859-1');
        $tpeid = 1;
	$clsFuncoes->exibirFinanceiroSubListaSSOSCANCELADA($tpeid, $tpEstimadaDetalhada = $_REQUEST['tpED'], $esdid = $_REQUEST['esdidOSAjax'],$ctrid= $_REQUEST['ctrid']);
  	exit;
}

function explodeSubOS($sql) {
    global $db;
    $cabecalho = array("Ação", "Nº OS", "Nº SS", "Detalhamento", "Prev. Início", "Prev. Término", "Qtd. PF", "Tipo de OS", "Situação");
    $tamanho = array("5%", "5%", "5%", "30%", "10%", "10%", "5%", "10%", "10%");
    $alinhamento = array("center", "center", "center", "left", "center", "center", "center", "center", "center");
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, "", $tamanho, $alinhamento);
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Painel Financeiro";
monta_titulo($titulo, '&nbsp;');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>


<script>

    function montaSubLista(esdid, tpED, ctrid)
    {
        td 	   = document.getElementById('td_'+esdid);
        img_mais   = document.getElementById('img_mais_'+esdid);
        img_menos   = document.getElementById('img_menos_'+esdid);

        ssdtini   = document.getElementById('ssdtini').value;
        ssdtfim   = document.getElementById('ssdtfim').value;

       // osdtini   = document.getElementById('osdtini').value;
       // osdtfim   = document.getElementById('osdtfim').value;


        // Faz uma requisição ajax, via POST
        $.ajax({type: "POST",
            url: "fabrica.php?modulo=principal/painelFinanceiro&acao=A",
            data: "esdidAjax="+esdid+"&tpED="+tpED+"&ssdtini="+ssdtini+"&ssdtfim="+ssdtfim+"&osdtini="+osdtini+"&osdtfim="+osdtfim+"&ctrid="+ctrid,
            success: function(msg){

                td.style.display = '';
                //td.style.padding = '5px';
                img_mais.style.display = 'none';
                img_menos.style.display = '';
                td.innerHTML = msg;
            }
        });

    }

    function desmontaSubLista(esdid, ctrid)
    {
        td 	   = document.getElementById('td_'+esdid);
        img_mais   = document.getElementById('img_mais_'+esdid);
        img_menos   = document.getElementById('img_menos_'+esdid);

        td.innerHTML = '';
        td.style.display = 'none';
        img_mais.style.display = '';
        img_menos.style.display = 'none';

    }


    function montaSubListaOS(esdid, tpED, tipoLista, ctrid)
    {

        td 	   = document.getElementById('td_'+tipoLista+esdid);
        img_mais   = document.getElementById('img_mais_'+tipoLista+esdid);
        img_menos   = document.getElementById('img_menos_'+tipoLista+esdid);

        ssdtini   = document.getElementById('ssdtini').value;
        ssdtfim   = document.getElementById('ssdtfim').value;
        
        // Faz uma requisição ajax, via POST
        $.ajax({type: "POST",
            url: "fabrica.php?modulo=principal/painelFinanceiro&acao=A",
            data: "esdidOSAjax="+esdid+"&tpED="+tpED+"&tipoLista="+tipoLista+"&ssdtini="+ssdtini+"&ssdtfim="+ssdtfim+"&ctrid="+ctrid,
            success: function(msg){

                td.style.display = '';
                //td.style.padding = '5px';
                img_mais.style.display = 'none';
                img_menos.style.display = '';
                td.innerHTML = msg;
            }
        });

    }

    function desmontaSubListaOS(esdid, tipoLista, ctrid)
    {
        td 	   = document.getElementById('td_'+tipoLista+esdid);
        img_mais   = document.getElementById('img_mais_'+tipoLista+esdid);
        img_menos   = document.getElementById('img_menos_'+tipoLista+esdid);

        td.innerHTML = '';
        td.style.display = 'none';
        img_mais.style.display = '';
        img_menos.style.display = 'none';

    }

//FUNÇÕES PARA LISTA SS E OS DA EMPRESA 1
//=============================================================================
    function montaSubListaSSOS(esdid, tpED, ctrid)
    {
        td 	   = document.getElementById('td_'+esdid);
        img_mais   = document.getElementById('img_mais_'+esdid);
        img_menos   = document.getElementById('img_menos_'+esdid);

        ssdtini   = document.getElementById('ssdtini').value;
        ssdtfim   = document.getElementById('ssdtfim').value;
        

       // osdtini   = document.getElementById('osdtini').value;
       // osdtfim   = document.getElementById('osdtfim').value;


        // Faz uma requisição ajax, via POST
        $.ajax({type: "POST",
            url: "fabrica.php?modulo=principal/painelFinanceiro&acao=A",
            data: "esdidAjax="+esdid+"&tpED="+tpED+"&ssdtini="+ssdtini+"&ssdtfim="+ssdtfim+"&osdtini="+osdtini+"&osdtfim="+osdtfim+"&SSOS=1"+"&ctrid="+ctrid,
            success: function(msg){

                td.style.display = '';
                //td.style.padding = '5px';
                img_mais.style.display = 'none';
                img_menos.style.display = '';
                td.innerHTML = msg;
            }
        });

    }

    function desmontaSubListaSSOS(esdid,ctrid)
    {
        td 	   = document.getElementById('td_'+esdid);
        img_mais   = document.getElementById('img_mais_'+esdid);
        img_menos   = document.getElementById('img_menos_'+esdid);

        td.innerHTML = '';
        td.style.display = 'none';
        img_mais.style.display = '';
        img_menos.style.display = 'none';

    }


    function montaSubListaSSOS(esdid, tpED, tipoLista, ctrid)
    {

        td 	   = document.getElementById('td_'+tipoLista+esdid);
        img_mais   = document.getElementById('img_mais_'+tipoLista+esdid);
        img_menos   = document.getElementById('img_menos_'+tipoLista+esdid);

        ssdtini   = document.getElementById('ssdtini').value;
        ssdtfim   = document.getElementById('ssdtfim').value;

        // Faz uma requisição ajax, via POST
        $.ajax({type: "POST",
            url: "fabrica.php?modulo=principal/painelFinanceiro&acao=A",
            data: "esdidOSAjax="+esdid+"&tpED="+tpED+"&tipoLista="+tipoLista+"&ssdtini="+ssdtini+"&ssdtfim="+ssdtfim+"&SSOS=1"+"&ctrid="+ctrid,
            success: function(msg){

                td.style.display = '';
                //td.style.padding = '5px';
                img_mais.style.display = 'none';
                img_menos.style.display = '';
                td.innerHTML = msg;
            }
        });

    }

    function desmontaSubListaSSOS(esdid, tipoLista, ctrid)
    {
        td 	   = document.getElementById('td_'+tipoLista+esdid);
        img_mais   = document.getElementById('img_mais_'+tipoLista+esdid);
        img_menos   = document.getElementById('img_menos_'+tipoLista+esdid);

        td.innerHTML = '';
        td.style.display = 'none';
        img_mais.style.display = '';
        img_menos.style.display = 'none';

    }

//===========================================================================

//FUNÇÕES PARA LISTA SS E OS CANCELADAS DA EMPRESA 1
//=============================================================================
    function montaSubListaSSOSCANCELADAS(esdid, tpED, ctrid)
    {
        td 	   = document.getElementById('td_'+esdid);
        img_mais   = document.getElementById('img_mais_'+esdid);
        img_menos   = document.getElementById('img_menos_'+esdid);

        ssdtini   = document.getElementById('ssdtini').value;
        ssdtfim   = document.getElementById('ssdtfim').value;

       // osdtini   = document.getElementById('osdtini').value;
       // osdtfim   = document.getElementById('osdtfim').value;


        // Faz uma requisição ajax, via POST
        $.ajax({type: "POST",
            url: "fabrica.php?modulo=principal/painelFinanceiro&acao=A",
            data: "esdidAjax="+esdid+"&tpED="+tpED+"&ssdtini="+ssdtini+"&ssdtfim="+ssdtfim+"&osdtini="+osdtini+"&osdtfim="+osdtfim+"&SSOSCANCELADAS=2"+"&ctrid="+ctrid,
            success: function(msg){

                td.style.display = '';
                //td.style.padding = '5px';
                img_mais.style.display = 'none';
                img_menos.style.display = '';
                td.innerHTML = msg;
            }
        });

    }

    function desmontaSubListaSSOSCANCELADAS(esdid,ctrid)
    {
        td 	   = document.getElementById('td_'+esdid);
        img_mais   = document.getElementById('img_mais_'+esdid);
        img_menos   = document.getElementById('img_menos_'+esdid);

        td.innerHTML = '';
        td.style.display = 'none';
        img_mais.style.display = '';
        img_menos.style.display = 'none';

    }


    function montaSubListaSSOSCANCELADAS(esdid, tpED, tipoLista, ctrid)
    {

        td 	   = document.getElementById('td_'+tipoLista+esdid);
        img_mais   = document.getElementById('img_mais_'+tipoLista+esdid);
        img_menos   = document.getElementById('img_menos_'+tipoLista+esdid);

        ssdtini   = document.getElementById('ssdtini').value;
        ssdtfim   = document.getElementById('ssdtfim').value;

        // Faz uma requisição ajax, via POST
        $.ajax({type: "POST",
            url: "fabrica.php?modulo=principal/painelFinanceiro&acao=A",
            data: "esdidOSAjax="+esdid+"&tpED="+tpED+"&tipoLista="+tipoLista+"&ssdtini="+ssdtini+"&ssdtfim="+ssdtfim+"&SSOSCANCELADAS=2"+"&ctrid="+ctrid,
            success: function(msg){

                td.style.display = '';
                //td.style.padding = '5px';
                img_mais.style.display = 'none';
                img_menos.style.display = '';
                td.innerHTML = msg;
            }
        });

    }

    function desmontaSubListaSSOSCANCELADAS(esdid, tipoLista,ctrid)
    {
        td 	   = document.getElementById('td_'+tipoLista+esdid);
        img_mais   = document.getElementById('img_mais_'+tipoLista+esdid);
        img_menos   = document.getElementById('img_menos_'+tipoLista+esdid);

        td.innerHTML = '';
        td.style.display = 'none';
        img_mais.style.display = '';
        img_menos.style.display = 'none';

    }

//===========================================================================


    function limpar(){
        document.formulario.ssdtini.value='';
        document.formulario.ssdtfim.value='';
       document.formulario.ctrid.value = '';
       document.formulario.tipoSituacao.value = 0;
       // document.formulario.osdtini.value='';
        //document.formulario.osdtfim.value='';
        document.formulario.submit();
    }

    function validar(){
        document.formulario.submit();
    }

    $(function() {
		$("#ctridSelect option").each(function(){

			if($(this).val()==2){
				
				$(this).attr('selected', 'selected');
			}
		});
   	});

</script>

<style>
    .TituloTabela{background-color: #C5C5C5;font-weight:bold}
    .center{text-align:center}
    .link{cursor: pointer;}
    .middle{vertical-align: middle;}
</style>

<form name="formulario" id="formulario" method="post" >
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr>
            <td class="SubtituloDireita" id="contrato" name="contrato" >Contrato:</td>
            <td>
                    <?php $sql = "SELECT distinct 
                                ctr.ctrid as codigo,
                                (ent.entnome ||' - ' || to_char(ctrdtinicio,'dd/MM/YYYY') || ' à ' || to_char(ctrdtfim,'dd/MM/YYYY')) as descricao
                        FROM
                                fabrica.contrato ctr
                        LEFT JOIN
                                fabrica.contratotiposervico cts
                                on cts.ctrid=ctr.ctrid 
                        LEFT JOIN
                                fabrica.tiposervico tps
                                on tps.tpsid=cts.tpsid
                        INNER JOIN
                                fabrica.contratosituacao cs
                                on cs.ctrid=ctr.ctrid and cs.ctsstatus='A'
                        INNER JOIN
                                fabrica.tiposituacaocontrato tsc
                                on tsc.tscid=cs.tscid and tsc.tscstatus='A'
                       INNER JOIN entidade.entidade ent
			        on ent.entid = ctr.entidcontratado and ent.entstatus = 'A'
                                and ctr.ctrstatus = 'A'
                                ORDER BY codigo asc"; 
                    
                    
                    
                    $ctrid = $_POST['ctrid'];
                    
                    if (empty($ctrid)) {
                            $ctrid = $EmAndamento;                        
                    }
                    ?>

                    <?=$db->monta_combo("ctrid",$sql,'S',"-- Selecione --",'',"",'', '', '','','', '', $ctrid); ?>
                 
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="42%">Período:</td>
            <td>
                <?= campo_data2('ssdtini', 'N', 'S', 'Data início SS', 'S', '', '', formata_data_sql($_REQUEST['ssdtini'])); ?>
                &nbsp;à&nbsp;
                <?= campo_data2('ssdtfim', 'N', 'S', 'Data Fim SS', 'S', '', '', formata_data_sql($_REQUEST['ssdtfim'])); ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Situação</td>
            <td>
                <?
                $ItemTipoSituacao[0] = 'Todos';
                $ItemTipoSituacao[1] = 'Em Desenvolvimento';
                $ItemTipoSituacao[2] = 'Utilizado';
                ?>
                <select name="tipoSituacao" id="tipoSituacao">
                    <?php foreach ($ItemTipoSituacao as $key => $valor) { ?>
                            <option value="<? echo $key ?>"  <? if ($_REQUEST['tipoSituacao'] == $key) {
                        echo 'selected = true';
                    } else {
                        echo 'Todos';
                    } ?> ><? echo $valor ?></option> 
                    <? } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td align="center" colspan="2">
                <input type="button" class="botao" value="Filtrar" onclick="validar();">
                &nbsp;&nbsp;
                <input type="button" class="botao" value="Limpar" onclick="limpar();">
            </td>
        </tr>
    </table>
</form>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="2" valign="top" class="TituloTabela center">EMPRESAS</td>

    </tr>
    <tr>
        <td colspan="2" valign="top" class="center"><?php $clsFuncoes->exibirFinanceiroEmpresas($ctrid)//exibirDemandasAprovadas() ?></td>
    </tr>
    <tr>
        <td colspan="2" valign="top" class="TituloTabela center">EMPRESA 1</td>
    </tr>
<!-- INICIA A APRESENTAÇÃO DA EMPRESA 1 !-->
    <tr>
        <td colspan="2" valign="top" class="center">
            <?php $clsFuncoes->exibirFinanceiroEmpresa1SSOS($ctrid) ?>
        </td>
    </tr>
    <tr>
        <td colspan="2" valign="top" class="TituloTabela center">CANCELADAS</td>
    </tr>
<!-- INICIA A APRESENTAÇÃO DA EMPRESA 1 !-->
    <tr>
        <td colspan="2" valign="top" class="center">
            <?php $clsFuncoes->exibirFinanceiroEmpresa1SSOSCANCELADA($ctrid) ?>
        </td>
    </tr>
    <tr>
        <td colspan="2" valign="top" class="TituloTabela center">EMPRESA 2</td>

    </tr>
    <tr>
        <td width="50%" class="TituloTabela center">OS - Ordem de Serviço - Contagem Estimada</td>
        <td class="TituloTabela center">OS - Ordem de Serviço - Contagem Detalhada</td>
    </tr>
    <tr>
        <td valign="top" class="center">
            <?php $clsFuncoes->exibirFinanceiroOS($tpeid = 2, $tpEstimadaDetalhada = 1, $tipoLista = 21, $ctrid) ?>
            <br>
            OS Cancelada
            <?php $clsFuncoes->exibirFinanceiroOS($tpeid = 2, $tpEstimadaDetalhada = 3, $tipoLista = 21, $ctrid) ?>
        </td>
        <td valign="top" class="center">
            <?php $clsFuncoes->exibirFinanceiroOS($tpeid = 2, $tpEstimadaDetalhada = 2, $tipoLista = 22, $ctrid) ?>
            <br>
            OS Cancelada
            <?php $clsFuncoes->exibirFinanceiroOS($tpeid = 2, $tpEstimadaDetalhada = 4, $tipoLista = 22, $ctrid) ?>
        </td>
    </tr>
</table>
