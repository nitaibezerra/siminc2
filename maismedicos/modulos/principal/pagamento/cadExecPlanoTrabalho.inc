<?php

if($_REQUEST['requisicao'] == 'carregaIfes'){

	$sql = "select
				ifeid as codigo,
				ifenome as descricao
			from maismedicos.ifes
			".($_GET["q"] ? "WHERE ifenome ILIKE '%{$_GET["q"]}%'" : "")."";

	$arDados = $db->carregar($sql);

	$q = strtolower($_GET["q"]);

	if($arDados){
		foreach ($arDados as $key=>$value){
			echo "{$value['descricao']}|{$value['codigo']}\n";
		}
	}else{
		echo "Sem registros|";
	}
	die;
}


if($_REQUEST['requisicao'] == 'carregaCampi'){

	$sql = "select
	ifcid as codigo,
	ifcnome as descricao
	from maismedicos.ifes_campi
	WHERE  ifeid = {$_REQUEST['ifeid']}";

	echo $db->monta_combo("ifccod_".$_REQUEST['tipo'], $sql, 'S', 'Selecione...', '', '', '', '100', 'N', 'ifcid_'.$_REQUEST['tipo'], '', '', '', '', '', '', true);
	die;
}

if($_REQUEST['requisicao']=='excluiItem' && $_REQUEST['id']){
	if($_REQUEST['tipo']=='visita'){
		$sqlD = "update maismedicos.planotrabalho_visitaprogramada set ptvstatus = 'I' where ptvid = {$_REQUEST['id']}";
	}else
		if($_REQUEST['tipo']=='atividade'){
		$sqlD = "update maismedicos.planotrabalho_atividadeacompanhamento set ptastatus = 'I' where ptaid = {$_REQUEST['id']}";
	}
	//echo $_REQUEST['tipo'].'<br/>';
	if($sqlD){
		//echo $sql;
		$db->executar($sqlD);
		$db->commit();
	}
	die;
}


if($_REQUEST['requisicao']=='downloadAnexo'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die;
}

include_once APPRAIZ . 'maismedicos/classes/Planotrabalho_Execucao.class.inc';
$obPte = new Planotrabalho_Execucao();
include_once APPRAIZ.'maismedicos/classes/Pagamento_Periodo.class.inc';
$obPer = new Pagamento_Periodo();

if($_REQUEST['requisicao']=='excluiExecucao'){
	$obPte->excluiExecucao($_GET['pteid']);
	$obPte->sucesso('principal/pagamento/cadExecPlanoTrabalho');
	die;
}

if($_REQUEST['requisicao']=='abreExecucao'){
	
	if($obPte->abreExecucao($_POST)) 
		die('abriu');

	die('erro');
}

if($_REQUEST['requisicao']=='salvaExecucao'){
	$obPte->salvaExecucaoPT($_POST, $_FILES);
	$obPte->sucesso('principal/pagamento/cadExecPlanoTrabalho', '');
	die;
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

monta_titulo('Informar Execução do Plano de Trabalho', '');
$db->cria_aba( $abacod_tela, $url, $parametros);

$periodo = $obPer->abrePreenchimentoExecucao($_REQUEST);
//echo '<pre>'; var_dump($periodo);die;

?>

<link rel="stylesheet" type="text/css" href="/includes/jquery-validate/css/validate.css" />

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/jquery.livequery.js"></script>

<script type="text/javascript" src="/includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="/includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="/includes/jquery-validate/lib/jquery.metadata.js"></script>

<script type="text/javascript" src="/includes/jquery-autocomplete/jquery.autocomplete.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/jquery-autocomplete/jquery.autocomplete.css" />

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>


<?php if($periodo['existe']): ?>

<table class="tabela" cellpadding="3" cellspacing="1" width="95%" align="center">
  <tr>
    <td bgcolor="">
   		<center><p><font size="+1" color="green">Execução n.º <?php echo str_pad($periodo['existe'],6,'0', STR_PAD_LEFT).', de '.mes_extenso($periodo['permes']).' de '.$periodo['perano'].''; ?>, enviada com sucesso.</font></p></center>
    </td>
  </tr>
</table>

<?php elseif($periodo['semplano']): ?>

<table class="tabela" cellpadding="3" cellspacing="1" width="95%" align="center">
  <tr>
    <td bgcolor="">
   		<center><p><font size="+1" color="red">Não existe Plano de Trabalho <b>Enviado</b> para o mês <?php echo $periodo['permes'].'/'.$periodo['perano']; ?>!</font></p></center>
    </td>
  </tr>
</table>

<?php elseif(!$periodo): ?>

<table class="tabela" cellpadding="3" cellspacing="1" width="95%" align="center">
  <tr>
    <td>
   		<center><p><font size="+1">Não existe execução do plano de trabalho aberta!</font></p></center>
    </td>
  </tr>
</table>
	
<?php else: ?>	

<style type="text/css">
                
    div#tabela
    {
        width: 95%;      /* Largura da minha tabela na tela */
        /*height: 250px; */    /* Altura da minha tabela na tela */
        <?php echo ($habilitado=='S') ? 'height: 400px;' : 'height: 280px;' ?>
        overflow: auto;    /* Barras de rolagem automáticas nos eixos X e Y */
        margin: 0 auto;    /* O 'auto' é para ficar no centro da tela */
        position:relative; /* Necessário para os cabecalhos fixos */
        top:0;             /* Necessário para os cabecalhos fixos */
        left:0;            /* Necessário para os cabecalhos fixos */
        border: 1px solid #f5f5f5;
    }
    div#tabela table {border-collapse:collapse; /* Sem espaços entre as células */}
    div#tabela table td
    {       
        border:1px solid white;
        height:30px;
        min-height:30px;        
    }
    div#tabela table#cabecalhoHorizontal td{background-color:buttonface;}    
    div#tabela table#cabecalhoHorizontal
    {
        position:absolute; /* Posição variável em relação ao topo da div#tabela */
        top:0;             /* Posição inicial em relação ao topo da div#tabela */
        z-index:5;         /* Para ficar por cima da tabela de dados */
    }
    div#tabela table#cabecalhoHorizontal > thead td 
    {
        BACKGROUND-COLOR: #dcdcdc;
        text-align:center;
        vertical-align:middle;
    }
    div#tabela table#cabecalhoHorizontal > tbody td{height: 66px;}    
    div#tabela table#dados
    {
        margin-top:96px;  /* 30px de altura do cabecalho horizontal + 2 pixels das bordas do cabecalho + 1 px*/
        z-index:2;        /* Menor que dos cabecalhos, para que fique por detrás deles */
    }
    div#tabela table#dados td{text-align:left;}
    label.filebutton 
    {
	    width:100%;    
	    overflow:hidden;
	    position:relative;
	    text-align:center;
	    margin-top:0px;
	}
	label span input 
	{
	    z-index: 999;
	    line-height: 0;
	    font-size: 50px;
	    position: absolute;
	    top: -2px;
	    left: -700px;
	    opacity: 0;
	    filter: alpha(opacity = 0);
	    -ms-filter: "alpha(opacity=0)";
	    cursor: pointer;
	    _cursor: hand;
	    margin: 0;
	    padding:0;
	}

</style>
<script>
	
	function enviarForm()
	{
	    var ifeid = $('[name=ifeid]');    
	    erro=false;

	    divCarregando();
	    
	    if(trim(ifeid.val())==''){
	        alert('O campo Instituição é obrigatório!');
	        ifeid.focus();
	        divCarregado();
	        return false;
	    }
	
	    $('#formulario').submit();
	}
	
	$(function(){
		
		$("[name=ptanome_atividade], [name=ptadsc_atividade], [name=ptvobs_visita], [name^='ptaobsexecutado'], [name^='ptvobsexecutado']").css({'width':'100%','height':'40px'});
		$("[name=ifenome_atividade], [name=ptatipo_atividade], [name=ifenome_visita]").css({'width':'100%'});

		$("[name^='ptadt']").css({'width':'90px'});
		$("[name^='ptatipo']").css({'width':'100%'});

		$("div#tabela").scroll(function () {
	        $('div#tabela #cabecalhoHorizontal, #versus').css('top', $(this).scrollTop());
	        $('div#tabela #cabecalhoVertical, #versus').css('left', $(this).scrollLeft());
	    });
		
		$("[name=ifenome_atividade]").autocomplete(document.location.href+'&requisicao=carregaIfes', {			
			matchContains: true,
			minChars: 0, 			
			cacheLength:1000,
			width: 440,
			autoFill: false,
			max: 1000    		
		}).result(function(event, data, formatted) {
			var ifeid = data[1];
			$('[name=ifecod_atividade]').val(ifeid);
			$.ajax({
				url		: document.location.href,
				type	: 'post',
				data	: '&requisicao=carregaCampi&tipo=atividade&ifeid='+ifeid,
				success	: function(e){
						$('#td_campi_atividade').html(e);
					}
				});
		});
	
		$("[name=ifenome_visita]").autocomplete(document.location.href+'&requisicao=carregaIfes', {			
			matchContains: true,
			minChars: 0, 			
			cacheLength:1000,
			width: 440,
			autoFill: false,
			max: 1000    		
		}).result(function(event, data, formatted) {
			var ifeid = data[1];
			$('[name=ifecod_visita]').val(ifeid);
			var ifeid = data[1];
			$('[name=ifecod_visita]').val(ifeid);
			$.ajax({
				url		: document.location.href,
				type	: 'post',
				data	: '&requisicao=carregaCampi&tipo=visita&ifeid='+ifeid,
				success	: function(e){
						$('#td_campi_visita').html(e);
					}
				});
		});
	
		$('.adiciona_atividade').click(function(){
		        
	        var num = $('#dados_atividade').find('tr').length;    
	        num = num ? parseInt(num) : 0;
	        
	        var ifenome = $('[name=ifenome_atividade]').val();
	        var ifeid = $('[name=ifecod_atividade]').val();
	        var ifcid = $('[name=ifccod_atividade]').val();
	        var ifcnome = $("[name=ifccod_atividade] option:selected").text();
	        var ptanome = $('[name=ptanome_atividade]').val();
	        var ptadtprevisto = $('[name=ptadtprevisto_visita]').val();
	        var ptatipo = $('[name=ptatipo_atividade]').val();
	        var ptadsc = $('[name=ptadsc_atividade]').val();
	        var ptadtexecutado = $('[name=ptadtexecutado_atividade]').val();
	        var ptaobsexecutado = $('[name=ptaobsexecutado_atividade]').val();

	        img_executado = ptadtexecutado ? '<center><img src="../imagens/check_p.gif" /></center>' : '';
	        
	        //alert(ifenome+'_'+ifeid+'_'+ptanome+'_'+ptatipo+'_'+ptaexecutado+'_'+ptadtexecutado+'_'+ptaobsexecutado);
	        
	        if(!ifenome || !ifeid || !ptanome || !ptatipo || !ptadtexecutado || !ptaobsexecutado){
	            alert('Todos os campos da atividade são obrigatórios!');
	            return false;
	        }

	        camposHidden = '<input type="hidden" name="ptaid[]" value="" id="ptaid_'+num+'" />';
	        camposHidden += '<input type="hidden" name="ptadtenvioexec[]" value="" id="ptadtenvioexec_'+num+'" />';
	        
	        html  = '<tr id="tr_atividade_'+num+'">';
	        
	        html += '<td width="8%">'+ifenome+camposHidden+'<input type="hidden" name="ifeid_atividade[]" value="'+ifeid+'" /></td>';
	        html += '<td width="8%">'+ifcnome+'<input type="hidden" name="ifcid_atividade[]" value="'+ifcid+'" /></td>';
	        html += '<td width="8%">'+ptanome+'<input type="hidden" name="ptanome[]" value="'+ptanome+'" /></td>';
	        html += '<td width="10%">-<input type="hidden" name="ptadtprevisto[]" value="" /></td>';
	        html += '<td width="7%">'+ptatipo+'<input type="hidden" name="ptatipo[]" value="'+ptatipo+'" /></td>';
	        html += '<td width="18%">'+(ptadsc ? ptadsc : '-')+'<input type="hidden" name="ptadsc[]" value="'+ptadsc+'" /></td>';
//	        html += '<td width="5%" align="center"><label class="filebutton"><img title="Incluir anexo" id="img_anexo_atividade_'+num+'" src="../imagens/gif_inclui.gif" /><span><input type="file" id="arquivo_atividade_'+num+'" name="arquivo_atividade[]"></span></label></td>';	        
	        html += '<td width="5%" align="center" valign="top">-</td>';	        

	        html += '<td width="10%">'+ptadtexecutado+'<input type="hidden" name="ptadtexecutado[]" value="'+ptadtexecutado+'" /></td>';
	        html += '<td width="16%">'+ptaobsexecutado+'<input type="hidden" name="ptaobsexecutado[]" value="'+ptaobsexecutado+'" /></td>';
	        html += '<td width="5%" valign="top" align="center"><label class="filebutton"><img title="Incluir anexo" id="img_anexo_execucao_atividade_'+num+'" src="../imagens/gif_inclui.gif" /><span><input type="file" id="arquivo_atividade_'+num+'" name="arquivo_atividade_execucao[]"></span></label></td>';
	        html += '<td width="5%" valign="top"><center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" /></center></td>';
	        
	        html += '</tr>';

	        $('#tr_nenhum_item_atividade').hide();
	        $('#dados_atividade > tbody').prepend(html);
	        $("[name=ifenome_atividade],[name=ifecod_atividade],[name=ptanome_atividade], [name=ptadtexecutado_atividade], [name=ptatipo_atividade], [name=ptadsc_atividade], [name=ptaobsexecutado_atividade]").val('');
	        $('#td_campi_atividade').html('');	        
		 });
	
		 $('.adiciona_visita').click(function(){
		        
	        var num = $('#dados_visita').find('tr').length;    
	        num = num ? parseInt(num) : 0;
	        
	        var ifenome = $('[name=ifenome_visita]').val();
	        var ifeid = $('[name=ifecod_visita]').val();
	        var ifcid = $('[name=ifccod_visita]').val();
	        var ifcnome = $("[name=ifccod_visita] option:selected").text();
	        var dtvisita = $('[name=ptvdtprevisto_visita]').val();
	        var ptvobs = $('[name=ptvobs_visita]').val();
	        var ptvdtexecini = $('[name=ptvdtexecini_visita]').val();
	        var ptvdtexecfim = $('[name=ptvdtexecfim_visita]').val();
	        var ptvobsexecutado = $('[name=ptvobsexecutado_visita]').val();

	        if(!ifeid){
		        alert('O campo Instituição é obrigatório!');
		        $('[name=ifenome_visita]').focus();
		        return false;
	        }

	        if(!ptvdtexecini){
	        	alert('O campo Período da execução é obrigatório!');
		        $('[name=ptvdtprevisto_visita]').focus();
		        return false;
	        }
	        
	        if(!ptvobsexecutado){
	        	alert('O campo Observação da execução é obrigatório!');
		        $('[name=ptvobsexecutado_visita]').focus();
		        return false;
	        }

			campoObs = '<textarea rows="1" cols="1" maxlength="5000" name="ptvobsexecutado[]" id="ptvobsexecutado_'+num+'" style="width:100%">'+ptvobsexecutado+'</textarea>';
	        campoData = '<input type="text" onblur="validando_data(this);MouseBlur(this);this.value=mascaraglobal(\'##/##/####\',this.value)" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onchange="" onkeyup="this.value=mascaraglobal(\'##/##/####\',this.value);" class="normal " maxlength="10" style="text-align: right;" size="12" value="'+ptvdtexecini+'" name="ptvdtexecini[]" id="ptvdtexecini_'+num+'">';
	        campoData += '&nbsp;<img border="0" align="absmiddle" onclick="displayCalendar(document.getElementById(\'ptvdtexecini[]\'),\'dd/mm/yyyy\',this)" title="Escolha uma Data" style="cursor:pointer" src="../includes/JsLibrary/date/displaycalendar/images/calendario.gif">';
	        campoData += '&nbsp;até<br/><input type="text" onblur="validando_data(this);MouseBlur(this);this.value=mascaraglobal(\'##/##/####\',this.value)" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onchange="" onkeyup="this.value=mascaraglobal(\'##/##/####\',this.value);" class="normal " maxlength="10" style="text-align: right;" size="12" value="'+ptvdtexecfim+'" name="ptvdtexecfim[]" id="ptvdtexecini_'+num+'">';
	        campoData += '&nbsp;<img border="0" align="absmiddle" onclick="displayCalendar(document.getElementById(\'ptvdtexecini[]\'),\'dd/mm/yyyy\',this)" title="Escolha uma Data" style="cursor:pointer" src="../includes/JsLibrary/date/displaycalendar/images/calendario.gif">';

	        camposHidden = '<input type="hidden" name="ptvid[]" value="" id="ptvid_'+num+'" />';
	        camposHidden += '<input type="hidden" name="ptvdtenvioexec[]" value="" id="ptvdtenvioexec_'+num+'" />';
	        
	        html  = '<tr id="tr_visita_'+num+'">';
	        html += '<td width="10%" valign="top">'+ifenome+camposHidden+'<input type="hidden" name="ifeid_visita[]" value="'+ifeid+'" /></td>';
	        html += '<td width="10%" valign="top">'+ifcnome+'<input type="hidden" name="ifcid_visita[]" value="'+ifcid+'" /></td>';
	        html += '<td width="12%" valign="top"><center>-</center></td>';
	        html += '<td width="20%" valign="top"><center>-</center></td>';
	        html += '<td width="5%" valign="top"><center>-</center></td>';
	        html += '<td width="12%" valign="top">'+campoData+'</td>';		        
	        html += '<td width="21%" valign="top">'+campoObs+'</td>';
	        html += '<td width="5%" valign="top" align="center"><label class="filebutton"><img title="Inserir anexo" id="img_anexo_visita_'+num+'" src="../imagens/gif_inclui.gif" /><span><input type="file" id="arquivo_visita_exec_'+num+'" name="arquivo_visita_exec[]"></span></label></td>';
	        html += '<td width="5%" valign="top"><center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" /></center></td>';
	        html += '</tr>';

	        $('#tr_nenhum_item_visita').hide();
	        $('#dados_visita > tbody').prepend(html);
	        $("[name=ifenome_visita], [name=ifecod_visita], [name=ptvdtprevisto_visita], [name=ptvobs_visita], [name=ptvdtexecini_visita], [name=ptvobsexecutado_visita]").val('');
	        $('#tr_add_visita_execucao').hide();
	        $('#dados_visita').css({'margin-top':'35px'});
// 	        $('#tr_view_add_visita_execucao').show();

	    });

	    $('[name=arquivo_visita_exec[]]').live('change',function(){
		    num = this.id.split('_')[3];		
		    if(this.value)	    
		    	$('#img_anexo_visita_exec_'+num).attr({'src': '../imagens/clipe.gif', 'title': this.value});		    
		});
		
	    $('[name=arquivo_atividade_execucao[]]').live('change',function(){
		    num = this.id.split('_')[3];		
		    if(this.value)	    
		    	$('#img_anexo_atividade_'+num).attr({'src': '../imagens/clipe.gif', 'title': this.value});		    
		});

		$('.btnDownloadAnexo').click(function(){
			url = document.location.href+'&requisicao=downloadAnexo&arqid='+this.id;
			var popUp = window.open(url, 'popDownloadAnexo', 'height=500,width=400,scrollbars=yes,top=50,left=200');
			popUp.focus();
		});

		$('.btnEnviarExecucao').click(function(){

			mes = $('[name=ptrmes]').val();
			semestre = $('[name=ptrsemestre]').val();
			ano = $('[name=ptrano]').val();

			var boErro = false;
			var txtErro = '';
			/*
			$.each($('[name=ptvdtexecini[]]'), function(i, e){
				if($(e).val()==''){boErro = true;alert('O campo data de execução da visita é obrigatório!');$(e).focus();}
			});			
			$.each($('[name=ptvdtexecfim[]]'), function(i, e){
				if($(e).val()==''){boErro = true;txtErro = 'O campo data de execução da atividade é obrigatório!';$(e).focus();}
			});
			*/
			
			if(boErro){ 
				return false;
			}

			var num_a = $('#dados_atividade > tbody').find('tr').length;    
			num_a = num_a ? parseInt(num_a) : 0;

			divCarregando();

			if(num_a<3){
				alert('A Execução do Plano de Trabalho deve conter ao menos duas atividades!');
				$('[name=ifenome_atividade]').focus();
				divCarregado();
				return false;
			}

			if(confirm('Deseja enviar a Execução do Plano de Trabalho do '+semestre+'º semestre de '+ano+'?')){
				$('[name=enviar_execucao]').val('true');
				$('#formulario').submit();
			}
		});

		$('[name=importa_execucao]').change(function(){
			if(confirm('Deseja importar as informações da Execução do Plano de Trabalho nº '+this.value+'?')){
				document.location.href = document.location.href+'&importa_execucao='+this.value; 
			}
		});

		$('.btnInsereVisitaNaoPlanejada').click(function(){
			if($(this).attr('src') == '../imagens/edit_on.gif'){
				$('#tr_add_visita_execucao').show();
				$('#dados_visita').css({'margin-top':'100px'});
				$(this).attr('src', '../imagens/edit_off.gif');
			}else{
				$('#tr_add_visita_execucao').hide();
				$('#dados_visita').css({'margin-top':'35px'});
				$(this).attr('src', '../imagens/edit_on.gif');
			}
		});
		
		$('.btnInsereAtividadeNaoPlanejada').click(function(){
			if($(this).attr('src') == '../imagens/edit_on.gif'){
				$('#tr_add_atividade_execucao').show();
				$('#dados_atividade').css({'margin-top':'100px'});
				$(this).attr('src', '../imagens/edit_off.gif');
			}else{
				$('#tr_add_atividade_execucao').hide();
				$('#dados_atividade').css({'margin-top':'35px'});
				$(this).attr('src', '../imagens/edit_on.gif');
			}
		});
	});

</script>

<?php 

$plano = $obPte->abreExecucao($periodo);
//echo '<pre>'; var_dump($plano);die;
$ptrid = $plano['ptrid'];
//ver($plano, d);

$habilitado = 'N';
if(empty($plano['ptedtenvio']) && $periodo['perid']==$plano['perid']){
	$habilitado = 'S';
}

extract($plano); 

?>

<?php if(empty($ptedtenvio)): ?>
	<center><div style="width:95%;background:#f0f0f0;border: 1px solid gray;padding: 5px;margin: 5px;">
		<p><font color="red" size="+1">Execução <b>não enviada</b>!</font></p>
		<p><font color="" size="+1"><?php $obPer->montaCabecalhoPeriodo($plano); ?></font></p></div></center>
<?php else: ?>
	<center><div style="width:95%;background:#f0f0f0;border: 1px solid gray;padding: 5px;margin: 5px;">
		<p><font color="green" size="+1">Execução <b>enviada com sucesso</b>.</font></p>
		<p><font color="" size="+1"><?php $obPer->montaCabecalhoPeriodo($plano); ?></font></p></div></center>
<?php endif; ?>

<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">	
	<input type="hidden" name="pteid" value="<?php echo $pteid; ?>" />
	<input type="hidden" name="ptrid" value="<?php echo $ptrid; ?>" />
	<input type="hidden" name="enviar_plano" value="" />
	<input type="hidden" name="enviar_execucao" value="" />
	<input type="hidden" name="ptrmes" value="<?php echo $ptemes; ?>" />
	<input type="hidden" name="ptrsemestre" value="<?php echo $ptesemestre; ?>" />
	<input type="hidden" name="ptrano" value="<?php echo $pteano; ?>" />
	<input type="hidden" name="requisicao" value="salvaExecucao" />
	<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<!-- 
	    <tr>
	        <td class="subtitulodireita" width="150" valign="top">Número do Plano ou Correção:</td>
	        <td valign="top"><?php echo str_pad($ptrid,10,'0', STR_PAD_LEFT); ?></td>
	    </tr>
	     -->
	    <tr>
	        <td class="subtitulodireita" width="150" valign="top">Código da Execução:</td>
	        <td valign="top"><?php echo str_pad($pteid,10,'0', STR_PAD_LEFT); ?></td>
	    </tr>
	    
	    <tr>
	        <td class="subtitulodireita" width="150" valign="top">Nome</td>
	        <td valign="top">
	        	<?php echo formatar_cpf($ptrcpf).' - '.$ptrnome; ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="subtitulodireita" valign="top">Período</td>
	        <td valign="top">
	        	<?php 
	        	echo $ptesemestre.'º semestre de '.$pteano.' ('.$ptemes.'/'.$pteano.')'; 
	        	?>
	        </td>
	    </tr>
	    
	    <tr>
			<td colspan="2" class="subtitulocentro">
				Visitas Programadas
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<center>
					<img align="absmiddle" title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">
					Indica campo obrigatório
				</center>
			</td>
		</tr>
	    <tr>
			<td colspan="2">
		
				<div id="tabela" style="width:100%">
					<table id="cabecalhoHorizontal" width="100%" bgcolor="#f5f5f5" align="center">
						<thead>
						    <tr>
						        <td width="10%">Instituição&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="10%">Campus&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="12%">Período da Visita</td>
						        <td width="20%">Observações da Visita</td>
						        <td width="5%">Anexo</td>
						        <td width="12%">Período da Execução&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="21%">Observação da Execução&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="5%">Anexo&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="5%"><?php if($habilitado=='S'): ?><img style="cursor:pointer" class="btnInsereVisitaNaoPlanejada" src="../imagens/edit_on.gif" alt="Inserir visita não prevista" title="Inserir visita não prevista" /><?php endif; ?></td>
						    </tr>
						</thead>
				    <?php if($habilitado=='S'): ?>
						<tbody>
							<tr id="tr_add_visita_execucao" style="display:none;">
							    <td valign="top" align="left">
							        <?=campo_texto('ifenome_visita','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="ifenome_visita"', '', '');?>
									<input type="hidden" name="ifecod_visita" id="ifecod_visita" value="" />
								</td>
							    <td valign="top" align="left" id="td_campi_visita">
							    
							    </td>
							    <td>
							    	<center>-</center>
							    </td>							    
							    <td valign="top" align="left">
							    	<center>-</center>
							    </td>
							    <td valign="top" align="left"><center>-</center></td>
							    <td valign="top" align="left">
				                	<?php echo campo_data2( 'ptvdtexecini_visita', 'N', $habilitado, '', 'N','','' );?>
				                	&nbsp;até<br/>
				                	<?php echo campo_data2( 'ptvdtexecfim_visita', 'N', $habilitado, '', 'N','','' );?>
							    </td>							    
							    <td valign="top" align="left">
							    	<?php echo campo_textarea( 'ptvobsexecutado_visita', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '','', array('id'=>'ptvobs_visita'));?>
							    </td>
							    <td valign="top" align="left">&nbsp;</td>
							    <td valign="middle" align="center">
							         <input type="button" class="adiciona_visita" alt="Incluir atividade" title="Incluir atividade" value="Incluir"> 
							    </td>
							</tr>
<!-- 							<tr id="tr_view_add_visita_execucao"> -->
<!-- 								<td colspan="9"><center><input type="button" value="Adicionar nova visita" class="btnInsereVisitaNaoPlanejada" /></center></td> -->
<!-- 							</tr> -->
						</tbody>
			        <?php endif; ?>                              
					</table>
				    <?php
				    $pteid = $planoExecucao ? $planoExecucao['pteid'] : $pteid;
				    $sql = "select * from maismedicos.planotrabalho_visitaprogramada v
						    left join maismedicos.ifes u on u.ifeid = v.ifeid 
						    left join maismedicos.ifes_campi c on c.ifcid = v.ifcid 
						    where ptvstatus = 'A'
						    and ptrid in (select ptrid from maismedicos.planotrabalho 
													where ptrstatus = 'A' and ptrcpf = '{$_SESSION['usucpf']}'
													and ptrsemestre = '{$plano['ptesemestre']}'
												) ";
				    //echo '<pre>';var_dump($plano);die; 
				    $arVisita = $db->carregar($sql);
				    ?>
					<table id="dados_visita" width="100%" bgcolor="#f5f5f5" style="margin-top:35px;">
						<tbody>
							<?php if($arVisita): ?>         
							    <?php foreach($arVisita as $key => $visita): ?>             
							        <tr id="tr_visita_<?php echo $key; ?>" style="<?php //echo $key%2 ? 'background-color: rgb(233, 233, 233)' : ''; ?>">
							            <td width="10%" valign="top" align="left">
							                <?php echo $visita['ifenome']; ?>
							                <input type="hidden" name="ptvid[]" value="<?php echo $visita['ptvid']; ?>" id="ptvid_<?php echo $key; ?>" />
							                <input type="hidden" name="ptvdtenvioexec[]" value="<?php echo $visita['ptvdtenvioexec']; ?>" id="ptvdtenvioexec_<?php echo $key; ?>" />
							                <input type="hidden" name="ifeid_visita[]" value="<?php echo $visita['ifeid']; ?>" id="ifeid_visita_<?php echo $key; ?>" />
							            </td>
							            <td width="10%" valign="top" align="left">
							            	<?php echo $visita['ifcnome']; ?>
							            	<input type="hidden" name="ifcid_visita[]" value="<?php echo $visita['ifcid']; ?>" id="ifcid_visita_<?php echo $key; ?>" />
							            </td>
							            <td width="12%" valign="top" align="left">
							                <?php echo formata_data($visita['ptvdtprevini']); ?>
							                &nbsp;até<br/>
							                <?php echo formata_data($visita['ptvdtprevini']); ?>
							            </td>
							            <td width="20%" valign="top" align="left">
							                <?php echo $visita['ptvobs']; ?>
							            </td>
							            <td width="5%" valign="top">
							            	<?php if($visita['arqid']): ?>
								            	<center>
								                	<img src="../imagens/clipe.gif" id="<?php echo $visita['arqid']; ?>" class="btnDownloadAnexo" style="cursor:pointer;"/>
								                </center>
							                <?php else: ?>
						                		<center>-</center>
							                <?php endif; ?>
							            </td>
									    <td width="12%" valign="top" align="left">
									    <?php if(empty($visita['ptvdtenvioexec'])): ?>
						                	<?php echo campo_data2( 'ptvdtexecini[]', 'N', $habilitado, '', 'N','','' , formata_data($visita['ptvdtexecini']), '', '', 'ptvdtexecini_'.$key);?>
						                	&nbsp;até<br/>
						                	<?php echo campo_data2( 'ptvdtexecfim[]', 'N', $habilitado, '', 'N','','', formata_data($visita['ptvdtexecfim']), '', '', 'ptvdtexecfim_'.$key);?>
						                <?php else: ?>
						                	<?php echo formata_data($visita['ptvdtexecini']).'&nbsp;até<br/>'.formata_data($visita['ptvdtexecfim']); ?>
						                <?php endif; ?>
									    </td>							    
									    <td width="21%" valign="top" align="left">
									    	
									    	
									    	<?php if(empty($visita['ptvdtenvioexec'])): ?>
							                	<?php //echo campo_textarea( 'ptvobsexecutado[]', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '','', array('id'=>'ptvobsexecutado_'+$key));?>
									    		<?php echo campo_textarea( 'ptvobsexecutado[]', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '',$visita['ptvobsexecutado'], '', 'ptvobsexecutado_visita_'.$key);?>
							                <?php else: ?>
							                	<?php echo $visita['ptvobsexecutado']; ?>
							                <?php endif; ?>
									    </td>
									    <td width="5%" valign="top" align="left">
									    	<?php if($visita['arqidexecutado']): ?>
								            	<center>
								                	<img src="../imagens/clipe.gif" id="<?php echo $visita['arqidexecutado']; ?>" class="btnDownloadAnexo" style="cursor:pointer;"/>
								                </center>
							                <?php else: ?>
							                	<?php if($habilitado=='S' && empty($visita['ptvdtenvioexec'])): ?>
							                	<label class="filebutton">
								                	<img title="Inserir anexo" id="img_anexo_visita_exec_<?php echo $key; ?>" src="../imagens/gif_inclui.gif" />
								                	<span>
								                		<input type="file" id="arquivo_visita_exec_<?php echo $key; ?>" name="arquivo_visita_exec[]">
								                	</span>
							                	</label>
							                	<?php else: ?>
							                		<center>-</center>
							                		<input style="display:none" type="file" id="arquivo_visita_exec_<?php echo $key; ?>" name="arquivo_visita_exec[]">
							                	<?php endif; ?>
							                <?php endif; ?>
									    </td>
							            <td width="5%" valign="top">
							                <center>            
					                    <?php if($habilitado=='S' && empty($visita['ptvdtenvioexec'])): ?>
							                        <img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" id="visita_<?php echo $visita['ptvid']?>" />
							            <?php elseif(!empty($visita['ptvdtenvioexec'])): ?>
							            		<img src="../imagens/check.jpg" title="Executado" />
							            <?php elseif(empty($visita['ptvdtenvioexec'])): ?>
							            		<img src="../imagens/exclui_p.gif" title="Não Executado" />
					                    <?php endif; ?>
							                </center>
							            </td>
							        </tr>
							     <?php endforeach; ?>
							     <tr id="tr_nenhum_item_visita" style="display:none;">
						            <td>
						                <b>
						                    Não há visitas programadas para o Execução do Plano de Trabalho.
						                    <?php if($habilitado=='S'): ?>
						                    <br> 
						                    Para adicionar preencha o formulário acima e clique no botão <i>incluir atividade</i>.
						                    <?php endif; ?>
						                </b>
						            </td>
						        </tr>
							<?php else: ?>
							        <tr id="tr_nenhum_item_visita">
							            <td>
							                <b>
							                    Não há visitas programadas para o Execução do Plano de Trabalho.
							                    <?php if($habilitado=='S'): ?>
							                    <br> 
							                    Para adicionar preencha o formulário acima e clique no botão <i>incluir atividade</i>.
							                    <?php endif; ?>
							                </b>
							            </td>
							        </tr>
							<?php endif; ?>
						</tbody>
					</table>
				
				</div> 
	
			</td>
		</tr>
		<tr>
			<td colspan="2">
				&nbsp;
			</td>
		</tr>
	    <tr>
			<td colspan="2" class="subtitulocentro">
				Outras Atividades de Acompanhamento das IFES
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<center>
					<img align="absmiddle" title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">
					Indica campo obrigatório
				</center>
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<div id="tabela" style="width:100%">
					<table id="cabecalhoHorizontal" width="100%" bgcolor="#f5f5f5" align="center">
						<thead>
						    <tr>
						        <td width="8%">Instituição&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="8%">Campus&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="8%">Atividade&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="10%">Data</td>
						        <td width="7%">Tipo&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="18%">Descrição da Atividade</td>
						        <td width="5%">Anexo</td>
						        <td width="10%">Data de Execução&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="16%">Observação da Execução&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="5%">Anexo&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="5%">
						        <?php if($habilitado=='S'): ?>
						        <img style="cursor:pointer" class="btnInsereAtividadeNaoPlanejada" src="../imagens/edit_on.gif" alt="Inserir visita não prevista" title="Inserir visita não prevista" />
						        <?php endif; ?>
						        </td>
						    </tr>
						</thead>
				    <?php if($habilitado=='S'): ?>
						<tbody>
							<tr id="tr_add_atividade_execucao" style="display:none;">
							    <td valign="top" align="left">
							        <?=campo_texto('ifenome_atividade','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="ifenome_atividade"', '', '');?>
									<input type="hidden" name="ifecod_atividade" id="ifecod_atividade" value="" />
							    </td>
							    <td valign="top" align="left" id="td_campi_atividade">
							        
							    </td>
							    <td valign="top" align="left">
				                	<?php //echo campo_textarea( 'ptanome_atividade', 'N', $habilitado, '', 1, 1, 255, '', '', '', '', '','', array('id'=>'ptanome_atividade'));?>
				                	<?php echo campo_texto( 'ptanome_atividade', 'N', $habilitado, '', 1, 255, '', '', '', '', '','', array('id'=>'ptanome_atividade'));?>
							    </td>
							    <td valign="top" align="left">
				                	<cemter>-</cemter>
							    </td>
							    <td valign="top" align="left">
							    	<?php 
							    	$arDados = array(
												array('codigo'=>'Distância','descricao'=>'Distância'),
												array('codigo'=>'Presencial','descricao'=>'Presencial'));
							    	echo $db->monta_combo("ptatipo_atividade", $arDados, 'S', 'Selecione...', '', '', '', '', 'N', 'ptatipo_atividade'); 
							    	?>
							    </td>
							    <td valign="top" align="left">
							    	<?php echo campo_textarea( 'ptadsc_atividade', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '','', '', 'ptadsc_atividade_');?>
							    </td>
							    <td valign="top" align="left">&nbsp;</td>
							    <td valign="top" align="left"><?php echo campo_data2( 'ptadtexecutado_atividade', 'N', $habilitado, '', 'N','','' );?></td>
							    <td valign="top" align="left"><?php echo campo_textarea( 'ptaobsexecutado_atividade', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '','', '', 'ptaobs_atividade_'+$key);?></td>
							    <td valign="top" align="left">&nbsp;</td>
							    <td valign="middle" align="center">
							         <input type="button" class="adiciona_atividade" alt="Incluir atividade" title="Incluir atividade" value="Incluir">
							    </td>
							</tr>
						</tbody>
			        <?php endif; ?>                               
					</table>
				    
				    
				    <?php 
				    $sql = "select * from maismedicos.planotrabalho_atividadeacompanhamento v
						    left join maismedicos.ifes u on u.ifeid = v.ifeid 
						    left join maismedicos.ifes_campi c on c.ifcid = v.ifcid 
						    where ptastatus = 'A'
				    		and ptrid in (select ptrid from maismedicos.planotrabalho 
													where ptrstatus = 'A' and ptrcpf = '{$_SESSION['usucpf']}'
													and ptrsemestre = '{$plano['ptesemestre']}'
												) ";
				    $arAtividade = $db->carregar($sql);
				    ?>
					<table id="dados_atividade" width="100%" bgcolor="#f5f5f5" style="margin-top:35px;">
						<tbody>
							<?php if($arAtividade): ?>         
							    <?php foreach($arAtividade as $key => $atividade): ?>             
							        <tr id="tr_atividade_<?php echo $key; ?>">
							            <td width="8%" valign="top" align="left">
							                <?php echo $atividade['ifenome']; ?>
							                <input type="hidden" name="ptaid[]" value="<?php echo $atividade['ptaid']; ?>" id="ptaid_<?php echo $key; ?>" />
							                <input type="hidden" name="ptadtenvioexec[]" value="<?php echo $atividade['ptadtenvioexec']; ?>" id="ptadtenvioexec_<?php echo $key; ?>" />
							                <input type="hidden" name="ifeid_atividade[]" value="<?php echo $atividade['ifeid']; ?>" id="ifeid_atividade_<?php echo $key; ?>" />
							            </td>
							            <td width="8%" valign="top" align="left">
							            	<?php echo $atividade['ifcnome']; ?>
							            	<input type="hidden" name="ifcid_atividade[]" value="<?php echo $atividade['ifcid']; ?>" id="ifcid_atividade_<?php echo $key; ?>" />
							            </td>
							            <td width="8%" valign="top" align="left">
							                <?php echo $atividade['ptanome']; ?>
							                <input type="hidden" name="ptanome[]" value="<?php echo $atividade['ptanome']; ?>" id="ptanome_<?php echo $key; ?>" />
							            </td>
							            <td width="10%" valign="top" align="left">
							                <?php echo $atividade['ptadtprevisto'] ? formata_data($atividade['ptadtprevisto']) : '-'; ?>
							            </td>
							            <td width="7%" valign="top" align="left">
							                <?php echo $atividade['ptatipo']; ?>
							                <input type="hidden" name="ptatipo[]" value="<?php echo $atividade['ptatipo']; ?>" id="ptatipo_<?php echo $key; ?>" />
							            </td>
							            <td width="18%" valign="top" align="left">
							                <?php echo $atividade['ptadsc']; ?>
							                <input type="hidden" name="ptadsc[]" value="<?php echo $atividade['ptadsc']; ?>" id="ptadsc_<?php echo $key; ?>" />
							            </td>
							            <td width="5%" valign="top">
							            	<?php if($atividade['arqid']): ?>
								            	<center>
								                	<img src="../imagens/clipe.gif" id="<?php echo $atividade['arqid']; ?>" class="btnDownloadAnexo" style="cursor:pointer;"/>
								                </center>
							                <?php else: ?>
						                		<center>-</center>
							                <?php endif; ?>
							            </td>
							            <td width="10%" valign="top">
							            	<?php if(empty($atividade['ptadtenvioexec'])): ?>
							            		<?php echo campo_data2( 'ptadtexecutado[]', 'N', $habilitado, '', 'N','','' , formata_data($atividade['ptadtexecutado']), '', '', 'ptadtexecutado_'.$key);?>
							            	<?php else: ?>
							            		<?php echo formata_data($atividade['ptadtexecutado']); ?>
							            	<?php endif; ?>
							            </td>
							            <td width="16%" valign="top">
							            	<?php if(empty($atividade['ptadtenvioexec'])): ?>
							            		<?php echo campo_textarea( 'ptaobsexecutado[]', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '',$atividade['ptaobsexecutado'], '', 'ptaobsexecutado_atividade_'.$key);?>
							            	<?php else: ?>
							            		<?php echo $atividade['ptaobsexecutado']; ?>
							            	<?php endif; ?>
							            </td>
							            <td width="5%" valign="top">
							            	<?php if($atividade['arqidexecutado']): ?>
								            	<center>
								                	<img src="../imagens/clipe.gif" id="<?php echo $atividade['arqidexecutado']; ?>" class="btnDownloadAnexo" style="cursor:pointer;"/>
								                </center>
							                <?php else: ?>
							                	<?php if($habilitado=='S' && empty($atividade['ptadtenvioexec'])): ?>
							                	<label class="filebutton">
								                	<img title="Inserir anexo" id="img_anexo_atividade_<?php echo $key; ?>" src="../imagens/gif_inclui.gif" />
								                	<span>
								                		<input type="file" id="arquivo_atividade_execucao_<?php echo $key; ?>" name="arquivo_atividade_execucao[]">
								                	</span>
							                	</label>
							                	<?php else: ?>
							                		<center>-</center>
							                		<input style="display:none" type="file" id="arquivo_atividade_execucao_<?php echo $key; ?>" name="arquivo_atividade_execucao[]">
							                	<?php endif; ?>
							                <?php endif; ?>
							            </td>
							            <td width="5%" valign="top">
							                <center>            
						                        <?php if($habilitado=='S' && empty($atividade['ptadtenvioexec'])): ?>
							                        <img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" id="atividade_<?php echo $atividade['ptaid']?>" />
									            <?php elseif(!empty($atividade['ptadtenvioexec'])): ?>
									            		<img src="../imagens/check.jpg" title="Executado" />
									            <?php elseif(empty($atividade['ptadtenvioexec'])): ?>
									            		<img src="../imagens/exclui_p.gif" title="Não Executado" />
							                    <?php endif; ?>
							                </center>
							            </td>
							        </tr>
							     <?php endforeach; ?>		
							     <tr id="tr_nenhum_item_atividade" style="display:none;">
						            <td>
						                <b>
						                    Não há atividades cadastradas para o Execução do Plano de Trabalho.
						                    <?php if($habilitado=='S'): ?>
						                    <br> 
						                    Para adicionar preencha o formulário acima e clique no botão <i>incluir atividade</i>.
						                    <?php endif; ?>
						                </b>
						            </td>
						        </tr>					     
							<?php else: ?>
							        <tr id="tr_nenhum_item_atividade">
							            <td>
							                <b>
							                    Não há atividades cadastradas para o Execução do Plano de Trabalho.
							                    <?php if($habilitado=='S'): ?>
							                    <br> 
							                    Para adicionar preencha o formulário acima e clique no botão <i>incluir atividade</i>.
							                    <?php endif; ?>
							                </b>
							            </td>
							        </tr>
							<?php endif; ?>
						</tbody>
					</table>
				
				</div> 
	
			</td>
		</tr>
		<?php if($ptrobs): ?>
		<tr>
			<td class="subtitulodireita" valign="top">Observações do Plano</td>
			<td valign="top"><?php echo $ptrobs; ?></td>
		</tr>
		<?php endif; ?>
		<tr>
			<td class="subtitulodireita" valign="top">Observações Da Execução</td>
			<td>
				<?php 
				if($habilitado=='S'){
					echo campo_textarea( 'pteobs', 'N', $habilitado, '', 50, 8, 5000, '', '', '', '', '','', array('id'=>'pteobs'));
				}else{
					echo $pteobs;
				}
				?>
			</td>
		</tr>
		<?php if($habilitado=='S'): ?>
			<tr>
				<td class="subtitulodireita"></td>
				<td class="subtituloesquerda">
					<input type="submit" value="Salvar" />
					<input type="button" value="Enviar" class="btnEnviarExecucao" />
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>
<?php endif; ?>
<p>&nbsp;</p>
<?php 
$arDados = $obPte->recuperaExecucaoPorPlano($ptrid);
?>
<?php if($arDados): ?>
	<h3>&nbsp;&nbsp;Execuções Não Enviadas</h3>
	<?php 
	$arDados = $arDados ? $arDados : array();
	$arrCabecalho = array('Ação', 'Período', 'Data de envio', 'Visitas', 'Atividades', 'Pago');
	$db->monta_lista($arDados,$arrCabecalho,50,10,'','','');
	?>
	<?php if($_GET['pteid']): ?>
		<center><p>&nbsp;</p><p><input type="button" value="Voltar" class="btnVoltar" /></p></center>
	<?php endif; ?>
<?php endif; ?>
<script>
	$(function(){
		$('.btnEditaExecucao').click(function(){
			document.location.href = '/maismedicos/maismedicos.php?modulo=principal/pagamento/cadExecPlanoTrabalho&acao=A&pteid='+this.id;
		});
		$('.btnVoltar').click(function(){
			document.location.href = '/maismedicos/maismedicos.php?modulo=principal/pagamento/cadExecPlanoTrabalho&acao=A';
		});
		$('.btnRemoverLinha').live('click',function(){
			if(confirm('Deseja remover o item?')){
				if(this.id){
					params = this.id.split('_');
					$.ajax({
						url	: document.location.href,
						type : 'post',
						data : '&requisicao=excluiItem&tipo='+params[0]+'&id='+params[1],
						success : function(e){
							alert('Operação realizada com sucesso!');
							}
						});
				}
				$(this).parent().parent().parent().remove();
			}
		});
		$('.btnExcluiExecucao').click(function(){
			if(confirm('Deseja excluir a Execução do Plano de Trabalho?')){
				document.location.href = document.location.href+'&requisicao=excluiExecucao&pteid='+this.id;
			}
		});
	});
</script>