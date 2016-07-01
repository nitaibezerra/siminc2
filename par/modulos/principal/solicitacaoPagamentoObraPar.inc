<?php

if ($_REQUEST['titleFor']) {
    if(!$_SESSION['par_var']['proid']) die("Processo não encontrado");
	
    if( $_REQUEST['tp'] == 2 ){
        $sql = "SELECT 
                    p.pagnumeroempenho as numero,
                    --p.pagvalorparcela as valor,
                    pobvalorpagamento as valor,
                    p.pagsituacaopagamento as situacao
                FROM 
                    par.processoobra po
                INNER JOIN par.empenho e on e.empnumeroprocesso = po.pronumeroprocesso and empstatus = 'A'
                LEFT JOIN par.pagamento p on p.empid = e.empid
                LEFT JOIN par.pagamentoobra pob on pob.pagid = p.pagid
                WHERE 
                    p.pagstatus = 'A' AND
                    po.prostatus = 'A'  and
                    e.empid != ".$_REQUEST['empid']." AND 
                    pob.preid = ".$_REQUEST['titleFor']." AND 
                    po.proid = ".$_SESSION['par_var']['proid'];

        $arEmpenho = $db->carregar( $sql );
        $arEmpenho = $arEmpenho ? $arEmpenho : array();		

        $html = '<table>
                    <tr>
                        <th>Nº Empenho</th>
                        <th> - Valor Pago</th>
                        <th> - Situação</th>
                    </tr>';
        if( $arEmpenho ){
            $excP = array();
            foreach ($arEmpenho as $empenho) {
                $html .= '<tr>
                            <td>'.$empenho['numero'].'</td>
                            <td align="right">'.number_format($empenho['valor'],2,',','.').'</td>
                            <td align="right">&nbsp;&nbsp;'.$empenho['situacao'].'</td>
                        </tr>';
            }
        } else {
            echo 'Sem pagamentos';
            die();
        }
        $html .= '</table>';
    } else {
        $sql = "SELECT 
                    --p.pagvalorparcela as valor,
                    pobvalorpagamento as valor,
                    p.pagsituacaopagamento as situacao
                FROM 
                    par.processoobra po
                INNER JOIN par.empenho e on e.empnumeroprocesso = po.pronumeroprocesso and empstatus = 'A'
                LEFT JOIN par.pagamento p on p.empid = e.empid
                LEFT JOIN par.pagamentoobra pob on pob.pagid = p.pagid
                WHERE 
                    p.pagstatus = 'A' AND
                    po.prostatus = 'A' and
                    e.empid = ".$_REQUEST['empid']." AND 
                    pob.preid = ".$_REQUEST['titleFor']." AND 
                    po.proid = ".$_SESSION['par_var']['proid'];

        $arEmpenho = $db->carregar( $sql );
        $arEmpenho = $arEmpenho ? $arEmpenho : array();	

        $html = '<table>
                    <tr>
                        <th> Valor Pago</th>
                        <th> - Situação</th>
                    </tr>';

        if( $arEmpenho ){
            $excP = array();
            foreach ($arEmpenho as $empenho) {
                $html .= '<tr>
                            <td align="right">'.number_format($empenho['valor'],2,',','.').'</td>
                            <td align="right">&nbsp;&nbsp;'.$empenho['situacao'].'</td>
                          </tr>';
            }
        } else {
            echo 'Sem pagamentos';
            die();
        }
        $html .= '</table>';
    }
    echo $html;
    die();
}

include  APPRAIZ."par/classes/Habilita.class.inc";
include '_funcoes_pagamentoObraPar.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

if($_REQUEST['proid']) {
    $_SESSION['par_var']['proid'] = $_REQUEST['proid'];
    $SQL = "SELECT esdid, CASE WHEN iu.itrid = 1 THEN 'Estado' ELSE 'Município' END as label FROM par.processoobraspar pro INNER JOIN par.instrumentounidade iu ON iu.inuid = pro.inuid INNER JOIN workflow.documento d ON d.docid = iu.docid WHERE  pro.prostatus = 'A' and proid = ".$_REQUEST['proid'];
    $esdid = $db->pegaLinha($sql); 
}

if($_REQUEST['empnumeroprocesso']) {
	$_SESSION['par_var']['empnumeroprocesso'] = $_REQUEST['empnumeroprocesso']; 
}

$processo = $_REQUEST['processo'];
$retorno = verificaEmpenhoValorDivergente($processo, '', 'OBRA');

if( !empty($retorno[0]['processo']) ){
    die("<script>
            alert('Para realizar o pagamento deste processo é necessário ajustar os dados de empenho.');
            window.close();
        </script>");
}

# Monta tarja vermelha com aviso de pendência de obras
montarAvisoCabecalho(null, null, null, null, $_REQUEST['proid'], 'obras');
?>
<html>
    <head>
        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
<body>
<?php
monta_titulo( $titulo_modulo, '<p align=center><b>SELECIONE UM EMPENHO</b> para visualizar os dados do pagamento</p>' );
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
    <script type="text/javascript" src="./js/par.js"></script>
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
    <style >
    .ui-dialog-titlebar{
        text-align: center;
    }
    </style>
    <script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
    <script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
    <script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
    <script type="text/javascript" src="/includes/remedial.js"></script>
    <script type="text/javascript" src="/includes/superTitle.js"></script>
    <link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
    <link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript">

function abrirDadosObras(preid, preano){
	window.open('par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&ano='+preano, 'obras','fullscreen=yes,scrollbars=yes' )
}

function carregarListaEmpenhoPagamento( empid ) {
    $.ajax({
            type: "POST",
            url: "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A",
            data: "requisicao=listaEmpenho&empid="+empid,
            async: false,
            success: function(msg){
                    document.getElementById('listapagamento').innerHTML = msg;
            }
        });
}

function verDadosPagamento(empid) {

    if( $('#preidobrid').val() == '' ) {
        var urlRes = "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A";
    } else {
    		if($('#sldid').val() != '')
    		{
    			var urlRes = "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A&preidobrid=" + $('#preidobrid').val() + "&sldid="+ $('#sldid').val();	
    		}
    		else
    		{
            	var urlRes = "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A&preidobrid=" + $('#preidobrid').val();
    		} 
    }
    divCarregando();
    $.ajax({
        type: "POST",
        url: urlRes,
        data: "requisicao=dadosPagamento&empid="+empid,
        async: false,
        success: function(msg){
            document.getElementById('dadospagamento').innerHTML = msg;
            divCarregado();
        }
    });
}

$(document).ready(function() {
    if( jQuery('#bopendenciaobra').val() == 'S' ){
        carregaPendenciaTermo( '<?php echo $_REQUEST['proid']; ?>' );
    } else {
    	if( $('#totPagSemComp').val() > 0 ){
    		redistribuirPagamentos();
    	}
    }
    carregarListaEmpenhoPagamento();

    
});

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow
	
function displayMessage(url) {
    var today = new Date();
    messageObj.setSource(url+'&hash='+today);
    messageObj.setCssClassMessageBox(false);
    messageObj.setSize(690,400);
    messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
    messageObj.display();
}

function closeMessage() {
    messageObj.close();	
}

function verObrasPagamento(pagid)
{		
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: "requisicao=obrasPagamento&pagid="+pagid,
        async: false,
        success: function(msg){
            jQuery('.ui-icon ui-icon-closethick').hide();
            jQuery( '#dialog-aut' ).hide();
            jQuery('#dialog-confirm').attr('title', 'Obras do Pagamento');
            jQuery( '#dialog-confirm' ).hide();
            jQuery( "#dialog-confirm" ).html(msg);
            jQuery( "#dialog-confirm" ).dialog({
                resizable: true,
                height:400,
                width:800,
                modal: true,
                show: { effect: 'drop', direction: "up" },
                buttons: {
                    "Fechar": function() {
                        jQuery( this ).dialog( "close" );
                    }					
                }
            });
        }
    });
    jQuery('.ui-dialog-titlebar-close').hide();	
}

function marcarPreObra(obj) {
    if(obj.checked) {
        obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].className='normal';
        obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].disabled=false;
        obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].className='normal';
        obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].disabled=false;
    } else {
        obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].className='disabled';
        obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].disabled=true;
        obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].value='';
        obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].className='disabled';
        obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].disabled=true;
        obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].value='';
    }
}

function cacularValorPagamento(obj, preid) {
	
    var tabela 					= obj.parentNode.parentNode.parentNode;
    var valorDigitado 			= replaceAll(replaceAll(obj.value, ".", ""),",", ".");
    var valorEmpenhoObra 		= obj.parentNode.parentNode.cells[4].childNodes[0].value;
    var valorPago 				= obj.parentNode.parentNode.cells[7].childNodes[2].value;
    var valorPagoNesteEmpenho 	= obj.parentNode.parentNode.cells[6].childNodes[2].value;
    var totalObra 				= obj.parentNode.parentNode.cells[5].childNodes[0].value;

    if(obj.id == 'valorpagamentoobra'){
        //Trata se valor pago maior que valor empenhado
        if( (parseFloat(valorDigitado)+parseFloat(valorPagoNesteEmpenho)) > parseFloat(valorEmpenhoObra) ){
            alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
            valorDigitado = parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho);
            if( valorDigitado < 0 ){
                valorDigitado = 0;
            }
            obj.value = mascaraglobal('[.###],##',valorDigitado.toFixed(2));
        }
        // FIM Trata se valor pago maior que valor empenhado

        // Trata se valor pago é maior que valor da Obra
        if( parseFloat(parseFloat(valorDigitado)+parseFloat(valorPago)) > parseFloat(totalObra) ) {
            alert('"Valor pagamento (R$)" + "Valor total pago (R$)" não pode ser maior que o "Valor da Obra (R$)"');
            if( (parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho)) > (parseFloat(totalObra)-parseFloat(valorPago)) ){
                valorDigitado = parseFloat(totalObra)-parseFloat(valorPago);
            }else{
                valorDigitado = parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho);
            }
            if( valorDigitado < 0 ){
                valorDigitado = 0;
            }
            obj.value = mascaraglobal('[.###],##',valorDigitado.toFixed(2));
        }
        // FIM Trata se valor pago é maior que valor da Obra

        var porcentPagamento = (parseFloat(valorDigitado) * 100) / parseFloat(totalObra);		
        obj.parentNode.parentNode.cells[11].childNodes[0].value = mascaraglobal('[#]',porcentPagamento.toFixed(0));

    } else {

        var porcentDigitado = parseFloat(replaceAll(obj.value, ",", "."));
        // Trata se Maior q 100%
        if( porcentDigitado > 100 ){
            porcentDigitado = 100;
            obj.value = 100;
        }
        // Fim Trata se Maior q 100%

        var valorPagamento = (porcentDigitado/100)*totalObra;

        //Trata se valor pago maoir que valor empenhado (%)
        if( parseFloat(parseFloat(valorPagamento)+parseFloat(valorPagoNesteEmpenho)) > parseFloat(valorEmpenhoObra) ){
            alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
            porcentDigitado = ((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100;
            if( porcentDigitado < 0 ){
                porcentDigitado = 0;
            }
            obj.value = porcentDigitado.toFixed(0);
        }
        // FIM Trata se valor pago maoir que valor empenhado

        // Trata se valor pago é maior que valor da Obra (%)
        if( parseFloat(porcentDigitado) > parseFloat( 100 - (parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100) ) ) {
            alert('"Valor pagamento (R$)" + "Valor total pago (R$)" não pode ser maior que o "Valor da Obra (R$)"');
            if( (((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100) > ((100-(parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100)) ) ){
                    porcentDigitado = (100-(parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100) );
            }else{
                porcentDigitado = ((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100;
            }
            if( porcentDigitado < 0 ){
                porcentDigitado = 0;
            }
            obj.value = porcentDigitado.toFixed(0);
        }
        // FIM Trata se valor pago é maior que valor da Obra (%)

        valorPagamento = (porcentDigitado/100)*totalObra;

        obj.parentNode.parentNode.cells[12].childNodes[0].value = mascaraglobal('[.###],##',valorPagamento.toFixed(2));

    }

    var totalPagamento=0;
    var falta=0;
    var valorTotalSeraPago = 0;
    var valorempenho = document.getElementById('valempid').value; //quanto falta pra pagar.
    var valorInformadoPago = $('[name="valorpagamentoobra['+preid+']"]').val(); //valor pago.
    valorInformadoPago = parseFloat(replaceAll(replaceAll(valorInformadoPago,".",""),",","."));

    var valorempenhado = $('[name="valorempenhado['+preid+']"]').val();
    valorempenhado = parseFloat(replaceAll(replaceAll(valorempenhado,".",""),",","."));

    var valorpagoempenho = $('[name="valorpagoempenho['+preid+']"]').val();
    valorpagoempenho = parseFloat(replaceAll(replaceAll(valorpagoempenho,".",""),",","."));

    for(var i=0;i<(tabela.rows.length)-3;i++) {
        if(tabela.rows[i].cells[12].childNodes[0].value) {
            totalPagamento += parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[12].childNodes[0].value,".",""),",","."));
        }
    }
	
    var emp = parseFloat(mascaraglobal('[###].##',parseFloat(valorempenho).toFixed(2)));
    var tot = parseFloat(replaceAll(replaceAll(document.getElementById('valorpagamento').value,".",""),",","."));

    if(totalPagamento > 0) {
        document.getElementById('solicitar').disabled=false;
        document.getElementById('valorpagamento').value = mascaraglobal('[.###],##',totalPagamento.toFixed(2));
        falta = valorempenho - totalPagamento;

        valorTotalSeraPago = parseFloat(totalPagamento); // + parseFloat(valorInformadoPago);

        if( parseFloat(falta.toFixed(0)) < 0 ){
            tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = '<span style="color:red;">Valor empenhado<br>ultrapassado em R$ '+mascaraglobal('[.###],##',falta.toFixed(2))+'</span>';
            document.getElementById('solicitar').disabled=true;
        } else {
            tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',falta.toFixed(2));
        }
    } else {
        document.getElementById('solicitar').disabled=true;
        document.getElementById('valorpagamento').value = '';
        tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',parseFloat(valorempenho).toFixed(2));
    }
}

function solPag() {
    var esdid = document.getElementById('esdid').value;
    var label = document.getElementById('label').value;
    var boTermoVenc = document.getElementById('boTermoVenc').value;

    if( boTermoVenc == 'S' ){
        alert('Não é possivel efetivar o pagamento. \nO termo desse processo encontra-se vencido.');
        return false;
    }else{
        if( esdid == 313 ){
            if(confirm("Este "+label+" se encontra em Diagnóstico. Deseja continuar?")){
                //document.getElementById('div_auth').style.display='block';
                $('.ui-icon ui-icon-closethick').hide();
                $( '#dialog-aut' ).hide();
                $( '#dialog-confirm' ).hide();
                $( '#dialog-aut' ).dialog({
                    resizable: false,
                    width: 450,
                    modal: true,
                    show: { effect: 'drop', direction: "up" },
                    buttons: {
                        'Ok': function() {
                            $( this ).dialog( 'close' );
                            //executaContaCorrente(codigo, tipo);
                            solicitarPagamento();
                        },
                        'Cancel': function() {
                            $( this ).dialog( 'close' );
                            //window.location.reload();
                        }
                    }
                });
            } else {
                return false;
            }
        } else if( esdid == 314 ){
            if(confirm("Este "+label+" se encontra em Elaboração. Deseja continuar?")){
                //document.getElementById('div_auth').style.display='block';
                $('.ui-icon ui-icon-closethick').hide();
                $( '#dialog-aut' ).hide();
                $( '#dialog-confirm' ).hide();
                $( '#dialog-aut' ).dialog({
                    resizable: false,
                    width: 450,
                    modal: true,
                    show: { effect: 'drop', direction: "up" },
                    buttons: {
                        'Ok': function() {
                            $( this ).dialog( 'close' );
                            //executaContaCorrente(codigo, tipo);
                            solicitarPagamento();
                        },
                        'Cancel': function() {
                            $( this ).dialog( 'close' );
                            //window.location.reload();
                        }
                    }
                });
            } else {
                return false;
            }
        } else {
            //document.getElementById('div_auth').style.display='block';
            $('.ui-icon ui-icon-closethick').hide();
            $( '#dialog-aut' ).hide();
            $( '#dialog-confirm' ).hide();
            $( '#dialog-aut' ).dialog({
                resizable: false,
                width: 450,
                modal: true,
                show: { effect: 'drop', direction: "up" },
                buttons: {
                    'Ok': function() {
                        $( this ).dialog( 'close' );
                        //executaContaCorrente(codigo, tipo);
                        solicitarPagamento();
                    },
                    'Cancel': function() {
                        $( this ).dialog( 'close' );
                        //window.location.reload();
                    }
                }
            });
        }
        jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
    }
}

function visPag(){
    var dados = $('#formPagamento').serialize();

    var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
    empid = empid.replace('=','');

    $.ajax({
        type: "POST",
        url: "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A",
        data: "requisicao=solicitarPagamento&tipo=visualiza&"+dados,
        async: false,
        success: function(msg){
            $('.ui-icon ui-icon-closethick').hide();
            $( '#dialog-aut' ).hide();
            $( '#dialog-confirm' ).hide();
            $( "#dialog-confirm" ).html(msg);
            $( "#dialog-confirm" ).dialog({
                resizable: false,
                height:700,
                width:800,
                modal: true,
                show: { effect: 'drop', direction: "up" },
                buttons: {
                    "OK": function() {
                        $( this ).dialog( "close" );
                        window.location.reload();								
                    }
                }
            });
        }
    });
}

function solicitarPagamento(){
    var form = document.getElementById('formPagamento');

    var usuario = document.getElementById('wsusuario').value;
    var senha   = document.getElementById('wssenha').value;
    var docvalidado   = document.getElementById('docvalidado').value;

    if(!usuario) {
        alert('Favor informar o usuário!');
        return false;
    }

    if(!senha) {
        alert('Favor informar a senha!');
        return false;
    }

    if(docvalidado == 'false'){
        alert('É necessário validar o documento para realizar o pagamento dessa(s) obra(s)!');
        return false;
    }

    var erroPreid = '';
	var erroDeferido = '';

	/*
	$('[name="preid[]"]').each(function(){
		var sldidcheck = $('[name="sldid['+$(this).val()+'][]"]:enabled:checked').length;
		var boInput = $('input[name="sldid['+$(this).val()+'][]"]').length;
		
		if( $(this).attr('checked') == true && sldidcheck == 0 && boInput > 0 ){
			erroDeferido += '->'+$('#td_nomeobra_'+$(this).val()).html()+'\n';
		}
	});*/
	
	if( erroDeferido != '' ){
		alert('Nas obras abaixo não foram informadas o % Deferido:\n'+erroDeferido);
		return false;
	}

    divCarregando();

    var dados = $('#formPagamento').serialize();
	
    var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
    empid = empid.replace('=','');

    $.ajax({
        type: "POST",
        url: "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A",
        data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarPagamento&"+dados,
        async: false,
        success: function(msg){
            $('.ui-icon ui-icon-closethick').hide();
            $( '#dialog-aut' ).hide();
            $( '#dialog-confirm' ).hide();
            $( "#dialog-confirm" ).html(msg);
            $( "#dialog-confirm" ).dialog({
                resizable: false,
                height:300,
                width:500,
                modal: true,
                show: { effect: 'drop', direction: "up" },
                buttons: {
                    "OK": function() {
                        $( this ).dialog( "close" );
                        window.location.reload();								
                    }
                }
            });
        }
    });

    document.getElementById('solicitar').disabled = false;
    divCarregado();
}

function carregarListaPagamentoEmpenho(empid,processo) {
    $.ajax({
        type: "POST",
        url: "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A",
        data: "requisicao=listaPagamentoEmpenho&empid=" + empid + "&empnumeroprocesso=" + processo,
        async: false,
        success: function(msg){
            if(!document.getElementById('listapagamentoprocesso')){
                document.getElementById('listapagamentoprocesso_' + processo).innerHTML = msg;
            }
            else{
                document.getElementById('listapagamentoprocesso').innerHTML = msg;
            }
        }
    });
}

function carregarHistoricoPagamento(pagid, obj) {
    var linha = obj.parentNode.parentNode;
    var tabela = obj.parentNode.parentNode.parentNode;

    if(obj.title == 'mais') {
        obj.title='menos';
        obj.src='../imagens/menos.gif';
        var nlinha = tabela.insertRow(linha.rowIndex);
        var ncol = nlinha.insertCell(0);
        ncol.colSpan=10;
        ncol.innerHTML="Carregando...";
        $.ajax({
            type: "POST",
            url: "par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A",
            data: "requisicao=listaHistoricoPagamento&pagid="+pagid,
            async: false,
            success: function(msg){
                ncol.innerHTML=msg;
            }
        });
    } else {
        obj.title='mais';
        obj.src='../imagens/mais.gif';
        var nlinha = tabela.deleteRow(linha.rowIndex);
    }
}

function carregaPendenciaTermo( proid ){
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: "requisicao=verificaPendenciaTermo&proid="+proid,
        async: false,
        success: function(msg){
            var closeOnEscape = jQuery( "#div_divergente" ).dialog( "option", "closeOnEscape" );
            jQuery( '#dialog-aut' ).hide();
            jQuery( '#dialog-confirm' ).hide();
            jQuery( "#dialog-confirm" ).html(msg);
            jQuery( "#dialog-confirm" ).dialog({
                resizable: false,
                closeOnEscape: false,
                height:300,
                width:500,
                modal: true,
                show: { effect: 'drop', direction: "up" },
                buttons: {
                    "OK": function() {
                            jQuery( this ).dialog( "close" );
                            window.close();							
                    }
                }
            });
        }
    });
    jQuery('.ui-dialog-titlebar-close').hide();
}
function mostraRestricoes(){
    $( "#dialog_restricoes" ).show();
    $( '#dialog_restricoes' ).dialog({
        resizable: false,
        width: 900,
        height: 600,
        modal: true,
        show: { effect: 'drop', direction: "up" },
        buttons: {
            'Fechar': function() {
                $( this ).dialog( 'close' );
            }
        }
    });
}
function mostraPendencia(){
    $( "#dialog_pendencia" ).show();
    $( '#dialog_pendencia' ).dialog({
        resizable: false,
        width: 900,
        height: 600,
        modal: true,
        show: { effect: 'drop', direction: "up" },
        buttons: {
            'Fechar': function() {
                $( this ).dialog( 'close' );
            }
        }
    });
}
</script>
<?php
$sql = "select count(p.pagid)
		from par.pagamento p
		  left join par.pagamentoobrapar o on p.pagid = o.pagid
		  inner join par.empenho e on e.empid = p.empid and e.empstatus = 'A'
		where p.pagstatus = 'A' and e.empnumeroprocesso = '$processo'
		group by e.empnumeroprocesso
		having count(o.popid) = 0";
$totPagSemComp = $db->pegaUm($sql);

cabecalhoSolicitacaoPagamentoObraPar();

$retornoHistorico = exibirHistoricoSigef($processo);
atualizaBasePagamentoSigef( $retornoHistorico, $processo, $_REQUEST['proid'] );

$_REQUEST['tooid'] = 2;
include_once APPRAIZ."par/modulos/principal/pagamento/redistribuicaoPagamentoObras.inc";
include_once APPRAIZ."par/modulos/principal/pagamento/deparaCorrecaoPagamentoEmpenhoObras.inc";

$arrValor = carregarPendenciaValorObras( $_REQUEST['proid'] );
$bopendenciaobra = 'N';
if( ((float)$arrValor['vrlempenhado'] > (float)$arrValor['dopvalortermo']) || ((float)$arrValor['vlrobras'] > (float)$arrValor['dopvalortermo']) ){
    $bopendenciaobra = 'S';
}

$sql = "SELECT 
            inu.inuid
        FROM par.processoobraspar p
        INNER JOIN territorios.municipio m ON m.muncod = p.muncod
        INNER JOIN par.instrumentounidade inu ON m.muncod = inu.muncod
        WHERE p.prostatus = 'A' and p.proid = {$_REQUEST['proid']}";
$arrDados = $db->pegaLinha( $sql );				
$inuId = $arrDados['inuid'];
if($inuId)
{
    $sqlRestricoes = 
            "SELECT 
                res.resdescricao,
                tpr.tprdescricao as tipo_restricao,
                CASE WHEN res.restemporestricao THEN
                    to_char(resdatainicio, 'DD/MM/YYYY')
                ELSE
                    '-'
                END
                as data_inicio,
                CASE WHEN res.restemporestricao THEN
                    to_char(resdatafim, 'DD/MM/YYYY')
                ELSE
                    '-'
                END as data_fim                        
            FROM par.restricaoentidade re
            INNER JOIN par.restricaofnde res ON re.resid = res.resid
            INNER JOIN par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
            LEFT JOIN workflow.documento doc ON doc.docid = re.docid 
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE re.inuid = {$inuId} 
                AND res.resstatus = 'A'
                AND esd.esdid NOT IN (".ESDID_CONCLUIDO.")";

    $arrRestricoes = $db->carregar($sqlRestricoes);
    $tooid = 2;
    $arrBloqueioObra = verificaBloqueioObras( $inuId, $tooid );

    $sql = "SELECT rstdsc, foo.obrid, o.obrnome
                FROM ( SELECT DISTINCT
                        esd.esdid as res_estado,
                        obrid,
                        r.rstdsc
                       FROM obras2.restricao r
                       INNER JOIN workflow.documento doc ON doc.docid = r.docid
                       INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                       WHERE obrid in (select pr.obrid from par.processoobraspar p 
                                          inner join par.processoobrasparcomposicao pc on pc.proid = p.proid  and pc.pocstatus = 'A'
                                          inner join obras2.obras pr on pr.preid = pc.preid AND pr.obrstatus = 'A' AND pr.obridpai IS NULL
                                      where p.prostatus = 'A' and p.proid = {$_REQUEST['proid']}
                                          and pr.obridpai is null) 
                                AND rstitem = 'R' AND rststatus = 'A'
                        ) as  foo
          inner join obras2.obras o on o.obrid = foo.obrid
                     where res_estado not in (1497, 1142, 1143)";
    $arrRestricoesSup = $db->carregar($sql);
    $arrRestricoesSup = $arrRestricoesSup ? $arrRestricoesSup : array();
    
    $boTermoVenc = pegaDataFimDocumentoParObra( $_REQUEST['processo'] );
   
    $arrDocVenc = array();
    if( $boTermoVenc == 'S' ){
        $sql = "SELECT 
                    d.dopdatafimvigencia,
                    m.mdonome
                FROM par.documentopar d
                    inner join par.modelosdocumentos m on m.mdoid = d.mdoid and m.mdostatus = 'A' AND d.dopstatus = 'A'
                WHERE
                    d.proid = {$_REQUEST['proid']}";
        $arrDocVenc =  $db->pegaLinha($sql);
    }

    if( (count($arrRestricoes) && is_array($arrRestricoes)) || ($arrBloqueioObra['boBloqueia']) || !empty($arrRestricoesSup[0]) ) {
?>
<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 98%;" cellpadding="3" cellspacing="1">
    <thead>
        <tr>
            <td  colspan="4"> Restrições do município </td>
        </tr>
    </thead>
    <tbody>
<?php
    if(count($arrRestricoes) && is_array($arrRestricoes))
    {
        foreach($arrRestricoes as $kRes => $vRes )
        {
            echo "<tr style=\"color:red;\">
                    <td style=\"width:40%;\" > {$vRes['resdescricao']} </td>
                </tr>";	
        }
    }
    if( $arrBloqueioObra['boBloqueia'] )
    {
            echo "<tr style=\"color:red;\">
                    <td style=\"width:40%;\" >";
            echo '<span style="color: red;">Existem pendências no Sistema '.$arrBloqueioObra['sistema'].'.</span><a href="#" onclick="mostraPendencia();";><span style="color: blue; cursor:pointer"> Clique aqui para verificar as pendências.</span></a>';
            echo "</td></tr>";	
    }

    if( !empty($arrRestricoesSup[0]) )
    {
        echo "<tr style=\"color:red;\">
                <td style=\"width:40%;\" >";
        echo '<span style="color: red;">Existem restrições não superadas no monitoramento de obras 2.0.</span><a href="#" onclick="mostraRestricoes();";><span style="color: blue; cursor:pointer"> Clique aqui para verificar as pendências.</span></a>';
        echo "</td></tr>";	
    }

    if( !empty($arrDocVenc[0]) )
    {
        echo "<tr style=\"color:red;\">
                <td style=\"width:40%;\" >";
        echo "<span style=\"color: red;\">Caso necessite realizar o pagamento desse processo, não será possível, pois o termo encontra-se com data de vigência expirada. <span style=\"color: blue;\">Termo: <?php echo {$arrDocVenc['mdonome']} &nbsp; Data Fim de Vigência: {$arrDocVenc['dopdatafimvigencia']}</span></a>";
        echo "</td></tr>";	
    }
?>
    </tbody>
</table>
<br>
<?php 
    }
} ?>
<style>
#listaRestricoes thead tr td {
    background-color: #F7F7F7;
    font-weight: bold;
    font-size: 12px;
    font-family: arial;
}
</style>
<form id="formPagamento">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<input type="hidden" name="totPagSemComp" id="totPagSemComp" value="<?=$totPagSemComp; ?>" />
	
    <input type="hidden" id="bopendenciaobra" name="bopendenciaobra" value="<?=$bopendenciaobra ?>" />
    <input type="hidden" id="esdid" name="esdid" value="<?=$esdid['esdid'] ?>" />
    <input type="hidden" id="label" name="label" value="<?=$esdid['label'] ?>" />
    <input type="hidden" name="tiposistema" id="tiposistema" value="OBRA" />
    <input type="hidden" id="boTermoVenc" name="boTermoVenc" value="<?=$boTermoVenc ?>" />
    <input type="hidden" id="preidobrid" name="preidobrid" value="<?php if($_REQUEST['preidobrid']){echo $_REQUEST['preidobrid'];}?>" />
    <input type="hidden" id="sldid" name="sldid" value="<?php if($_REQUEST['sldid']){echo $_REQUEST['sldid'];}?>" />
    <tr>
        <td><input type="button" value="Redistribuir Pagamentos" name="btnredistribuirPagamento" onclick="redistribuirPagamentos()" /></td>
    </tr>
    <tr>
        <td id="listapagamento">Carregando...</td>
    </tr>
    <tr>
        <td id="dadospagamento"></td>
    </tr>
</table>
</form>
<div id="debug"></div>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth";

/* $usuario = 'MECTHIAGOBARBOSA'; 
$senha   = '45389408'; */
?>
<div class="demo">
    <div id="dialog-confirm" title="Solicitação de Pagamento" style="overflow-x: auto; overflow-y: auto; width:100%; height:250px; font-size: 12px;"></div>
</div>

<div id="dialog-aut" title="Autenticação" style="display:none;">
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td width="30%" style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Usuário:</td>
            <td>					
                <input type="text" title="Usuário" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
                        onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$usuario ?>" size="23" id="wsusuario" name="wsusuario">
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Senha:</td>
            <td>
                <input type="password" class="obrigatorio normal"  title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
                        onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$senha ?>" size="23" id="wssenha" name="wssenha">
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                <input type="hidden" name="ws_pagid" id="ws_pagid" value="" />
                <input type="hidden" name="ws_processo" id="ws_processo" value="" />
            </td>
        </tr>
    </table>
    <div id="resposta" style="color: red; font-size: 0.9em"></div>
</div>
<div id="debug"></div>
<div id="dialog_pendencia" title="Pendências <?=$arrBloqueioObra['sistema']; ?>" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt; color: red;">
	<?php
	if(is_array($arrBloqueioObra) && count($arrBloqueioObra))
	{
		foreach ($arrBloqueioObra['pendencia'] as $obra => $pendencia) {
			echo ($obra + 1).' - '.$pendencia;
		}	
	}
	?>
	</div>
</div>

<div id="dialog_desembolso" title="Solicitação de Desembolso" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt;" id="dialog_retorno"></div>
</div>

<div id="dialog_restricoes" title="Restrições não Superadas" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt; color: red;">
	<?php
	if(is_array($arrRestricoesSup) && count($arrRestricoesSup))
	{
		foreach ($arrRestricoesSup as $obra => $restricao) {
			echo ($obra + 1).' - Obra: ('. $restricao['obrid'] . ') ' . $restricao['obrnome'] . ' - Restrição: ' . $restricao['rstdsc'].'<br>';
		}	
	}
	?>
	</div>
</div>