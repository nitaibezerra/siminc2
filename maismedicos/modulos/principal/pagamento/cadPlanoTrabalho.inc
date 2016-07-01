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

if($_REQUEST['requisicao']=='downloadAnexo'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die;
}

include_once APPRAIZ . 'maismedicos/classes/Planotrabalho.class.inc';
$obPt = new Planotrabalho();
include_once APPRAIZ.'maismedicos/classes/Pagamento_Periodo.class.inc';
$obPer = new Pagamento_Periodo();

if($_REQUEST['requisicao']=='excluiPlano'){
	$obPt->excluiPlano($_GET['ptrid']);
	$obPt->sucesso('principal/pagamento/cadPlanoTrabalho');
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

if($_REQUEST['requisicao']=='abrePlano'){
	
	if($obPt->verificaPlanoTrabalhoMesAno($_REQUEST['mes'], $_REQUEST['ano'], $_REQUEST['semestre']))
		die('existe');
		
	if($obPt->abrePlano($_POST)) 
		die('abriu');

	die('erro');
}

if($_REQUEST['requisicao']=='salvaPlano'){
	$obPt->salvaPlanoCoordAvaliador($_POST, $_FILES);
	$obPt->sucesso('principal/pagamento/cadPlanoTrabalho', '');
	die;
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

monta_titulo('Plano de Trabalho', '');
$db->cria_aba( $abacod_tela, $url, $parametros);

$periodo = $obPer->abrePreenchimentoPlano();
?>

<link rel="stylesheet" type="text/css" href="/includes/jquery-validate/css/validate.css" />

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<!-- <script type="text/javascript" src="js/jquery.livequery.js"></script>  -->

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
   		<center><p><font size="+1" color="green">Plano N.º <?php echo str_pad($periodo['existe'],6,'0', STR_PAD_LEFT).' ('.$periodo['permes'].'/'.$periodo['perano'].')'; ?> enviado com sucesso.</font></p></center>
    </td>
  </tr>
</table>

<?php elseif(!$periodo && !$_GET['ptrid']): ?>

<table class="tabela" cellpadding="3" cellspacing="1" width="95%" align="center">
  <tr>
    <td>
   		<center><p><font size="+1">Não existe preenchimento aberto!</font></p></center>
    </td>
  </tr>
</table>
	
<?php else: ?>	

<?php 

$plano = $obPt->abrePlano($periodo, $_GET['ptrid']);

$habilitado = 'N';
if(empty($plano['ptrdtenvio']) && ( $periodo['perdtini']<=date('Y-m-d') && $periodo['perdtfim']>=date('Y-m-d')) ){
	$habilitado = 'S';
}

?>

<style type="text/css">
                
    div#tabela
    {
        width: 95%;      /* Largura da minha tabela na tela */
        /*height: 250px; */    /* Altura da minha tabela na tela */
        <?php echo ($habilitado=='S') ? 'height: 250px;' : 'height: 130px;' ?>
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
	    margin-top:5px;
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
	        alert('O campo IFES é obrigatório!');
	        ifeid.focus();
	        divCarregado();
	        return false;
	    }
	
	    $('#formulario').submit();
	}
	
	$(function(){
		
		$("[name=ptanome_atividade], [name=ptadsc_atividade], [name=ptvobs_visita]").css({'width':'100%','height':'40px'});
		$("[name=ifenome_atividade], [name=ptatipo_atividade], [name=ifenome_visita], , [name=ifcnome_visita]").css({'width':'100%'});
		$("[name=ptadtprevisto_visita]").css({'width':'90px'});

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
	        
	        if(!ifenome || !ifeid || !ptanome || !ptatipo || !ptadsc || !ptadtprevisto){
	            alert('Todos os campos da atividade são obrigatórios!');
	            return false;
	        }

	        campoHiddem = '<input type="hidden" name="ptaid[]" value="" id="ptaid_'+num+'" />';

	        html  = '<tr id="tr_atividade_'+num+'">';
	        html += '<td width="20%">'+ifenome+campoHiddem+'<input type="hidden" name="ifeid_atividade[]" value="'+ifeid+'" /></td>';
	        html += '<td width="10%">'+ifcnome+'<input type="hidden" name="ifcid_atividade[]" value="'+ifcid+'" /></td>';
	        html += '<td width="20%">'+ptanome+'<input type="hidden" name="ptanome[]" value="'+ptanome+'" /></td>';
	        html += '<td width="11%">'+ptadtprevisto+'<input type="hidden" name="ptadtprevisto[]" value="'+ptadtprevisto+'" /></td>';
	        html += '<td width="9%">'+ptatipo+'<input type="hidden" name="ptatipo[]" value="'+ptatipo+'" /></td>';
	        html += '<td width="20%">'+ptadsc+'<input type="hidden" name="ptadsc[]" value="'+ptadsc+'" /></td>';
	        html += '<td width="5%" align="center"><label class="filebutton"><img title="Incluir anexo" id="img_anexo_atividade_'+num+'" src="../imagens/gif_inclui.gif" /><span><input type="file" id="arquivo_atividade_'+num+'" name="arquivo_atividade[]"></span></label></td>';	        
	        html += '<td width="5%"><center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" /></center></td>';
	        html += '</tr>';

	        $('#tr_nenhum_item_atividade').hide();
	        $('#dados_atividade > tbody').prepend(html);
	        $("[name=ifenome_atividade],[name=ifecod_atividade],[name=ptanome_atividade], [name=ptadtprevisto_visita], [name=ptatipo_atividade], [name=ptadsc_atividade]").val('');
	        $('#td_campi_atividade').html('');	        
		 });
	
		 $('.adiciona_visita').click(function(){
		        
	        var num = $('#dados_visita').find('tr').length;    
	        num = num ? parseInt(num) : 0;
	        
	        var ifenome = $('[name=ifenome_visita]').val();
	        var ifeid = $('[name=ifecod_visita]').val();
	        var ifcid = $('[name=ifccod_visita]').val();
	        var ifcnome = $("[name=ifccod_visita] option:selected").text();
	        var dtvisitaini = $('[name=ptvdtprevini_visita]').val();
	        var dtvisitafim = $('[name=ptvdtprevfim_visita]').val();
	        var ptvobs = $('[name=ptvobs_visita]').val();
	        
	        if(!ifenome || !ifeid || !dtvisitaini || !ptvobs){
	            alert('Todos os campos da atividade são obrigatórios!');
	            return false;
	        }

	        campoHiddem = '<input type="hidden" name="ptvid[]" value="" id="ptvid_'+num+'" />';
	        
	        html  = '<tr id="tr_visita_'+num+'">';
	        html += '<td width="20%">'+ifenome+campoHiddem+'<input type="hidden" name="ifeid_visita[]" value="'+ifeid+'" /></td>';
	        html += '<td width="15%">'+ifcnome+'<input type="hidden" name="ifcid_visita[]" value="'+ifcid+'" /></td>';
	        html += '<td width="20%">'+dtvisitaini+' até <input type="hidden" name="ptvdtprevini[]" value="'+dtvisitaini+'" />';
	        html += ''+dtvisitafim+'<input type="hidden" name="ptvdtprevfim[]" value="'+dtvisitafim+'" /></td>';
	        html += '<td width="35%">'+ptvobs+'<input type="hidden" name="ptvobs[]" value="'+ptvobs+'" /></td>';
	        html += '<td width="5%" align="center"><label class="filebutton"><img title="Inserir anexo" id="img_anexo_visita_'+num+'" src="../imagens/gif_inclui.gif" /><span><input type="file" id="arquivo_visita_'+num+'" name="arquivo_visita[]"></span></label></td>';		        
	        html += '<td width="5%"><center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" /></center></td>';
	        html += '</tr>';

	        $('#tr_nenhum_item_visita').hide();
	        $('#dados_visita > tbody').prepend(html);
	        $("[name=ifenome_visita], [name=ifecod_visita], [name=ptvdtprevini_visita], [name=ptvdtprevfim_visita], [name=ptvobs_visita]").val('');
	        $('#td_campi_visita').html('');	
	    });

	    $('[name=arquivo_visita[]]').live('change',function(){
	    //$('[name=arquivo_visita[]]').change(function(){
		    num = this.id.split('_')[2];		
		    if(this.value)	    
		    	$('#img_anexo_visita_'+num).attr({'src': '../imagens/clipe.gif', 'title': this.value});		    
		});
		
	    $('[name=arquivo_atividade[]]').live('change',function(){
	    //$('[name=arquivo_atividade[]]').change(function(){
		    num = this.id.split('_')[2];		
		    if(this.value)	    
		    	$('#img_anexo_atividade_'+num).attr({'src': '../imagens/clipe.gif', 'title': this.value});		    
		});

		$('.btnDownloadAnexo').click(function(){
			url = document.location.href+'&requisicao=downloadAnexo&arqid='+this.id;
			var popUp = window.open(url, 'popDownloadAnexo', 'height=500,width=400,scrollbars=yes,top=50,left=200');
			popUp.focus();
		});

		$('.btnEnviarPlano').click(function(){

			mes = $('[name=ptrmes]').val();
			semestre = $('[name=ptrsemestre]').val();
			ano = $('[name=ptrano]').val();

			var num_a = $('#dados_atividade > tbody').find('tr').length;    
			num_a = num_a ? parseInt(num_a) : 0;

			divCarregando();

			if(num_a<3){
				alert('O Plano de Trabalho deve conter ao menos duas atividades!');
				$('[name=ifenome_atividade]').focus();
				divCarregado();
				return false;
			}

			if(confirm('Deseja enviar o Plano de Trabalho do '+semestre+'º semestre de '+ano+'?')){
				$('[name=enviar_plano]').val('true');
				$('#formulario').submit();
			}
		});

		$('[name=importa_plano]').change(function(){
			if(confirm('Deseja importar as informações do Plano de Trabalho nº '+this.value+'?')){
				document.location.href = document.location.href+'&importa_plano='+this.value; 
			}
		});
	});

</script>

<?php
extract($plano); 
?>

<?php if(empty($ptrdtenvio)): ?>
	<center><div style="width:95%;background:#f0f0f0;border: 1px solid gray;padding: 5px;margin: 5px;">
		<p><font color="red" size="+1"><?php// echo $obPt->verificaPlanoSemestre($plano) ? '' : ''; ?>Plano de trabalho <b>não enviado</b>!</font></p>
		<p><font color="" size="+1"><?php $obPer->montaCabecalhoPeriodo($plano); ?></font></p></div></center>
<?php else: ?>
	<center><div style="width:95%;background:#f0f0f0;border: 1px solid gray;padding: 5px;margin: 5px;">
		<p><font color="green" size="+1">Plano de trabalho <b>enviado com sucesso</b>.</font></p>
		<p><font color="" size="+1"><?php $obPer->montaCabecalhoPeriodo($plano); ?></font></p></div></center>
<?php endif; ?>

<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">	
	<input type="hidden" name="ptrid" value="<?php echo $ptrid; ?>" />
	<input type="hidden" name="enviar_plano" value="" />
	<input type="hidden" name="perid" value="<?php echo $periodo['perid']; ?>" />
	<input type="hidden" name="ptrmes" value="<?php echo $ptrmes; ?>" />
	<input type="hidden" name="ptrsemestre" value="<?php echo $ptrsemestre; ?>" />
	<input type="hidden" name="ptrano" value="<?php echo $ptrano; ?>" />
	<input type="hidden" name="ptrcpf" value="<?php echo $ptrcpf; ?>" />
	<input type="hidden" name="requisicao" value="salvaPlano" />
	<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<!--  
	    <tr>
	        <td class="subtitulodireita" width="150">Número do Plano:</td>
	        <td><?php echo str_pad($ptrid,10,'0', STR_PAD_LEFT); ?></td>
	    </tr>
	    -->
	    <tr>
	        <td class="subtitulodireita">Período</td>
	        <td>
	        	<?php 
	        	echo $ptrsemestre.'º semestre de '.$ptrano.' ('.$ptrmes.'/'.$ptrano.')'; 
	        	
	        	?>
	        </td>
	    </tr>
	    <tr>
	        <td class="subtitulodireita" width="150">Alterações:</td>
	        <td><?php echo $obPt->contaPlanosPorPeriodo(array('ptrsemestre'=>$plano['ptrsemestre'])); ?></td>
	    </tr>
	    <tr>
	        <td class="subtitulodireita" width="150">Nome</td>
	        <td>
	        	<?php echo formatar_cpf($ptrcpf).' - '.$ptrnome; ?>
	        </td>
	    </tr>
	    
	    <tr>
			<td colspan="2" class="subtitulocentro">
				Visitas Programadas
			</td>
		</tr>
	    <tr>
			<td colspan="2">
		
				<div id="tabela" style="width:100%">
					<table id="cabecalhoHorizontal" width="100%" bgcolor="#f5f5f5" align="center">
						<thead>
						    <tr>
						        <td width="20%">Instituição&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="15%">Campus&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="20%">Período da visita&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="35%">Observações&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="5%">Anexo</td>
						        <td width="5%">&nbsp;</td>
						    </tr>
						</thead>
				    <?php if($habilitado=='S'): ?>
						<tbody>
							<tr>
							    <td valign="top" align="left">
							        <?=campo_texto('ifenome_visita','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="ifenome_visita"', '', '');?>
									<input type="hidden" name="ifecod_visita" id="ifecod_visita" value="" />
							    </td>
							    <td valign="top" align="left" id="td_campi_visita">
									
							    </td>
							    <td valign="top" align="left">
				                	<?php echo campo_data2( 'ptvdtprevini_visita', 'N', $habilitado, '', 'N','','' );?>
				                	&nbsp;até&nbsp;
				                	<?php echo campo_data2( 'ptvdtprevfim_visita', 'N', $habilitado, '', 'N','','' );?>
							    </td>							    
							    <td valign="top" align="left">
							    	<?php echo campo_textarea( 'ptvobs_visita', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '','', array('id'=>'ptvobs_visita'));?>
							    </td>
							    <td valign="top" align="left">&nbsp;</td>
							    <td valign="middle" align="center">
							         <input type="button" class="adiciona_visita" alt="Incluir atividade" title="Incluir atividade" value="Incluir"> 
							    </td>
							</tr>
						</tbody>
			        <?php endif; ?>                              
					</table>
				    <?php 
				    $ptrid = $plano['ptrid'];
				    $sql = "select * from maismedicos.planotrabalho_visitaprogramada v
						    left join maismedicos.ifes u on u.ifeid = v.ifeid 
						    left join maismedicos.ifes_campi c on c.ifcid = v.ifcid 
						    where ptvstatus = 'A'
    						and ptrid in (select ptrid from maismedicos.planotrabalho
	    						    where ptrstatus = 'A' and ptrcpf = '{$_SESSION['usucpf']}'
	    						    and ptrsemestre = '{$plano['ptrsemestre']}'
				    			)";
				    
				    $arVisita = $db->carregar($sql);
				    //echo '<pre>'; var_dump($arVisita);
				    ?>
					<table id="dados_visita" width="100%" bgcolor="#f5f5f5" style="<?php echo $habilitado=='S' ? 'margin-top:100px;' : 'margin-top:35px;'; ?>">
						<tbody>
							<?php if($arVisita): ?>         
							    <?php foreach($arVisita as $key => $visita): ?>             
							        <tr id="tr_visita_<?php echo $key; ?>" style="<?php //echo $key%2 ? 'background-color: rgb(233, 233, 233)' : ''; ?>">
							            <td width="20%" valign="top" align="left">
							                <?php echo $visita['ifenome']; ?>
							                <input type="hidden" name="ptvid[]" value="<?php echo $visita['ptvid']; ?>" id="ptvid_<?php echo $key; ?>" />
							                <input type="hidden" name="ifeid_visita[]" value="<?php echo $visita['ifeid']; ?>" id="ifeid_visita_<?php echo $key; ?>" />
							            </td>
							            <td width="15%" valign="top" align="left">
							                <?php echo $visita['ifcnome']; ?>
							                <input type="hidden" name="ifcid_visita[]" value="<?php echo $visita['ifcid']; ?>" id="ifcid_visita_<?php echo $key; ?>" />
							            </td>
							            <td width="20%" valign="top" align="left">
							                <?php echo formata_data($visita['ptvdtprevini']); ?>
							                &nbsp;até<br/>
							                <?php echo formata_data($visita['ptvdtprevfim']); ?>
							                <input type="hidden" name="ptvdtprevini[]" value="<?php echo formata_data($visita['ptvdtprevini']); ?>" id="ptvdtprevini_<?php echo $key; ?>" />
							                <input type="hidden" name="ptvdtprevfim[]" value="<?php echo formata_data($visita['ptvdtprevfim']); ?>" id="ptvdtprevfim_<?php echo $key; ?>" />
							            </td>
							            <td width="35%" valign="top" align="left">
							                <?php echo $visita['ptvobs']; ?>
							                <input type="hidden" name="ptvobs[]" value="<?php echo $visita['ptatipo']; ?>" id="ptvobs_<?php echo $key; ?>" />
							            </td>
							            <td width="5%">
							            	<?php if($visita['arqid']): ?>
								            	<center>
								                	<img src="../imagens/clipe.gif" id="<?php echo $visita['arqid']; ?>" class="btnDownloadAnexo" style="cursor:pointer;"/>
								                </center>
							                <?php else: ?>
							                	<?php if($habilitado=='S' && empty($visita['ptvdtenvioexec'])): ?>
							                	<label class="filebutton">
								                	<img title="Inserir anexo" id="img_anexo_visita_<?php echo $key; ?>" src="../imagens/gif_inclui.gif" />
								                	<span>
								                		<input type="file" id="arquivo_visita_<?php echo $key; ?>" name="arquivo_visita[]">
								                	</span>
							                	</label>
							                	<?php else: ?>
							                		<center>-</center>
							                		<input style="display:none;" type="file" id="arquivo_visita_<?php echo $key; ?>" name="arquivo_visita[]">
							                	<?php endif; ?>
							                <?php endif; ?>
							            </td>
							            <td width="5%" valign="middle">
							                <center>            
						                    <?php if($habilitado=='S' && empty($visita['ptvdtenvioexec'])){ ?>
								                        <img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" id="visita_<?php echo $visita['ptvid']; ?>" />
					                        <?php }else if(!empty($visita['ptvdtenvioexec'])){ ?>
					                        	<img src="../imagens/check_checklist.png" title="Executado" />
						                    <?php } ?>
							                </center>
							            </td>
							        </tr>
							     <?php endforeach; ?>
							     <tr id="tr_nenhum_item_visita" style="display:none;">
						            <td>
						                <b>
						                    Não há visitas programadas para o Plano de Trabalho.
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
							                    Não há visitas programadas para o Plano de Trabalho.
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
				<div id="tabela" style="width:100%">
					<table id="cabecalhoHorizontal" width="100%" bgcolor="#f5f5f5" align="center">
						<thead>
						    <tr>
						        <td width="20%">Instituição&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="10%">Campus&nbsp;<img title="Indica campo obrigatório." alt="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">&nbsp;</td>
						        <td width="20%">Nome da Atividade&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="11%">Data&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="9%">Tipo&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="20%">Descrição da Atividade&nbsp;<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0"></td>
						        <td width="5%">Anexo</td>
						        <td width="5%">&nbsp;</td>
						    </tr>
						</thead>
				    <?php if($habilitado=='S'): ?>
						<tbody>
							<tr>
							    <td valign="top" align="left">
							        <?=campo_texto('ifenome_atividade','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="ifenome_atividade"', '', '');?>
									<input type="hidden" name="ifecod_atividade" id="ifeid_atividade" value="" />
							    </td>
							    <td valign="top" align="left" id="td_campi_atividade">
							    </td>
							    <td valign="top" align="left">
				                	<?php echo campo_textarea( 'ptanome_atividade', 'N', $habilitado, '', 1, 1, 255, '', '', '', '', '','', array('id'=>'ptanome_atividade'));?>
							    </td>
							    <td valign="top" align="left">
				                	<?php echo campo_data2( 'ptadtprevisto_visita', 'N', $habilitado, '', 'N','','' );?>
							    </td>
							    <td valign="top" align="left">
							    	<?php 
							    	$arDados = array(
												array('codigo'=>'Distância','descricao'=>'Distância'),
												array('codigo'=>'Presencial','descricao'=>'Presencial'));
							    	echo $db->monta_combo("ptatipo_atividade", $arDados, 'S', 'Selecione...', '', '', '', '80', 'N', 'ptatipo_atividade'); 
							    	?>
							    </td>
							    <td valign="top" align="left">
							    	<?php echo campo_textarea( 'ptadsc_atividade', 'N', $habilitado, '', 1, 1, 5000, '', '', '', '', '','', array('id'=>'ptadsc_atividade'));?>
							    </td>
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
											and ptrsemestre = '{$plano['ptrsemestre']}'
										) 
							";
				    
				    $arAtividade = $db->carregar($sql);
				    ?>
					<table id="dados_atividade" width="100%" bgcolor="#f5f5f5" style="<?php echo $habilitado=='S' ? 'margin-top:100px;' : 'margin-top:35px;'; ?>">
						<tbody>
							<?php if($arAtividade): ?>         
							    <?php foreach($arAtividade as $key => $atividade): ?>             
							        <tr id="tr_atividade_<?php echo $key; ?>">
							            <td width="20%" valign="top" align="left">
							                <?php echo $atividade['ifenome']; ?>
							                <input type="hidden" name="ptaid[]" value="<?php echo $atividade['ptaid']; ?>" id="ptaid_<?php echo $key; ?>" />
							                <input type="hidden" name="ifeid_atividade[]" value="<?php echo $atividade['ifeid']; ?>" id="ifeid_atividade_<?php echo $key; ?>" />
						                </td>
						                <td width="10%" valign="top" align="left">
						                	<?php echo $atividade['ifcnome']; ?>
							                <input type="hidden" name="ifcid_atividade[]" value="<?php echo $atividade['ifcid']; ?>" id="ifcid_atividade_<?php echo $key; ?>" />
						                </td>
							            <td width="20%" valign="top" align="left">
							                <?php echo $atividade['ptanome']; ?>
							                <input type="hidden" name="ptanome[]" value="<?php echo $atividade['ptanome']; ?>" id="ptanome_<?php echo $key; ?>" />
							            </td>
							            <td width="11%" valign="top" align="left">
							                <?php echo formata_data($atividade['ptadtprevisto']); ?>
							                <input type="hidden" name="ptadtprevisto[]" value="<?php echo formata_data($atividade['ptadtprevisto']); ?>" id="ptadtprevisto_<?php echo $key; ?>" />
							            </td>
							            <td width="9%" valign="top" align="left">
							                <?php echo $atividade['ptatipo']; ?>
							                <input type="hidden" name="ptatipo[]" value="<?php echo $atividade['ptatipo']; ?>" id="ptatipo_<?php echo $key; ?>" />
							            </td>
							            <td width="20%" valign="top" align="left">
							                <?php echo $atividade['ptadsc']; ?>
							                <input type="hidden" name="ptadsc[]" value="<?php echo $atividade['ptadsc']; ?>" id="ptadsc_<?php echo $key; ?>" />
							            </td>
							            <td width="5%">
							            	<?php if($atividade['arqid']): ?>
								            	<center>
								                	<img src="../imagens/clipe.gif" id="<?php echo $atividade['arqid']; ?>" class="btnDownloadAnexo" style="cursor:pointer;"/>
								                </center>
							                <?php else: ?>
							                	<?php if($habilitado=='S' && empty($atividade['ptadtexecutado'])): ?>
							                	<label class="filebutton">
								                	<img title="Inserir anexo" id="img_anexo_atividade_<?php echo $key; ?>" src="../imagens/gif_inclui.gif" />
								                	<span>
								                		<input type="file" id="arquivo_atividade_<?php echo $key; ?>" name="arquivo_atividade[]">
								                	</span>
							                	</label>
							                	<?php else: ?>
							                		<center>-</center>
							                		<input style="display:none;" type="file" id="arquivo_atividade_<?php echo $key; ?>" name="arquivo_atividade[]">
							                	<?php endif; ?>
							                <?php endif; ?>
							            </td>
							            
							            <td width="5%" valign="middle">
							                <center>            
							            	<?php if($habilitado=='S' && empty($atividade['ptadtenvioexec'])): ?>
						                        <img src="../imagens/excluir.gif" border="0" style="cursor:pointer" class="btnRemoverLinha" alt="Remover" title="Remover" id="atividade_<?php echo $atividade['ptaid']; ?>"/>
							                <?php else: ?>
							                	<img src="../imagens/check_checklist.png" title="Executado" />
							                <?php endif; ?>
							                </center>
							            </td>
							        </tr>
							     <?php endforeach; ?>		
							     <tr id="tr_nenhum_item_atividade" style="display:none;">
						            <td>
						                <b>
						                    Não há atividades cadastradas para o Plano de Trabalho.
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
							                    Não há atividades cadastradas para o Plano de Trabalho.
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
			<td class="subtitulodireita" valign="top">Observações</td>
			<td>
				<?php 
				if($habilitado=='S'){
					echo campo_textarea( 'ptrobs', 'N', $habilitado, '', 50, 8, 5000, '', '', '', '', '','', array('id'=>'ptrobs'));
				}else{
					echo $ptrobs;
				}
				?>
			</td>
		</tr>
		
			<tr>
				<td class="subtitulodireita"></td>
				<td class="subtituloesquerda">
					<?php if($habilitado=='S'): ?>
						<input type="submit" value="Salvar" />
					<?php endif; ?>
					<input type="button" value="Enviar" class="btnEnviarPlano" />
					<?php if($_GET['ptrid']): ?>
						<input type="button" value="Voltar" class="btnVoltarPlano" />
					<?php endif; ?>
				</td>
			</tr>
		
	</table>
</form>
<?php endif; ?>
<p>&nbsp;</p>
<?php 
$arDados = $obPt->recuperaPlanosPorCpf($_SESSION['usucpf']);
?>
<?php if($arDados): ?>
<h3>&nbsp;&nbsp;<?php echo $obPt->verificaPlanoSemestre($plano) ? 'Alteração(ões) não Enviada(s)' : 'Plano de Trabalho não Enviado'; ?></h3>
<?php 
$arDados = $arDados ? $arDados : array();
$arrCabecalho = array('Ação', 'Mês Ref.', 'Preenchimento', 'Período',  'Data de envio', 'Visitas', 'Atividades', 'Pago');
$db->monta_lista($arDados,$arrCabecalho,50,10,'','','');
?>
<?php endif; ?>
<script>
	$(function(){
		$('.btnEditaPlano').click(function(){
			document.location.href = '/maismedicos/maismedicos.php?modulo=principal/pagamento/cadPlanoTrabalho&acao=A&ptrid='+this.id;
		});
		$('.btnVoltarPlano').click(function(){
			document.location.href = '/maismedicos/maismedicos.php?modulo=principal/pagamento/cadPlanoTrabalho&acao=A';
		});
		$('.btnRemoverLinha').live('click',function(){
		//$('.btnRemoverLinha').click(function(){
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
		$('.btnExcluiPlano').click(function(){
			if(confirm('Deseja excluir o Plano de Trabalho?')){
				document.location.href = document.location.href+'&requisicao=excluiPlano&ptrid='+this.id;
			}
		});
	});
</script>