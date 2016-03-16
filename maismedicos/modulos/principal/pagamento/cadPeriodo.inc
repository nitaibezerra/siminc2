<?php 

include_once APPRAIZ.'maismedicos/classes/Pagamento_Periodo.class.inc';
$obPer = new Pagamento_Periodo();

if($_REQUEST['requisicao']){
	$obPer->$_REQUEST['requisicao']($_POST);
	die;
}



require_once APPRAIZ . "includes/cabecalho.inc";

echo '<br/>';

$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo('Calendário de Preenchimento', 'Escolha uma atividade abaixo para gerenciar');

$habilitado = 'S';
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

<?php if($_REQUEST['atpid']): ?>

<script>
$(function(){
	$('.btnSalvar').click(function(){

		if(!$('[name=atpid]').val()){
			alert('O campo Atividade é obrigatório!');
			$('[name=atpid]').focus();
			return false;
		}
		/*if(!$('[name=pflcod]').val()){
			alert('O campo Perfil é obrigatório!');
			$('[name=pflcod]').focus();
			return false;
		}*/
		if(!$('[name=permes]').val()){
			alert('O campo Mês de Referência é obrigatório!');
			$('[name=permes]').focus();
			return false;
		}
		if(!$('[name=perano]').val()){
			alert('O campo Ano de Referência é obrigatório!');
			$('[name=perano]').focus();
			return false;
		}
		if(!$('[name=perdtini]').val()){
			alert('O campo Data de Início é obrigatório');
			$('[name=perdtini]').focus();
			return false;
		}
		if(!$('[name=perdtfim]').val()){
			alert('O campo Data de Fim é obrigatório');
			$('[name=perdtfim]').focus();
			return false;
		}
		
		$.ajax({
			url		: document.location.href,
			type	: 'post',
			data	: '&requisicao=salvaPeriodo&'+$('#formulario').serialize(),
			success	: function(e){
				//alert(e);
				document.location.href = document.location.href;
			}
		});
	});
	$('.btnVolta').click(function(){
		document.location.href = 'maismedicos.php?modulo=principal/pagamento/cadPeriodo&acao=A';
	});
	$('.btnEditar').click(function(){

		perid = this.id; 
		
		$.ajax({
			url		 : document.location.href,
			type	: 'post', 
			dataType : "json",
			data	: '&requisicao=recuperaPeriodo&perid='+perid,
			success	: function(e){
				//alert(e);
				$('[name=perid]').val(e.perid);
				$('[name=atpid]').val(e.atpid);
				$('[name=permes]').val(e.permes);
				$('[name=perano]').val(e.perano);
				$('[name=perdtini]').val(e.dtini);
				$('[name=perdtfim]').val(e.dtfim);
				$('[name=perobs]').val(e.perobs);
				}
			});
		//alert(this.id+'_');
	});
	
	$('[name=perano], [name=permes]').change(function(){

		mes = $('[name=permes]').val();
		ano = $('[name=perano]').val();
		perid = $('[name=perid]').val();

		if(mes && ano){
		$.ajax({
			url		: document.location.href,
			type	: 'post',
			data	: '&requisicao=verificaMesRefExiste&'+$('#formulario').serialize(),
			success	: function(e){
				//alert(e);
				if(e=='1' || e==1){
					alert('Já existe um mês cadastrado para '+mes+'/'+ano+'!');
					$('[name=perano], [name=permes]').val('');
					$('[name=permes]').focus();
				}
			}
		});
		}
		
	});
	$('[name=atpid]').change(function(){
		document.location.href = 'maismedicos.php?modulo=principal/pagamento/cadPeriodo&acao=A&atpid='+this.value;
	});
});
</script>

<form name="formulario"id="formulario"  method="post" action="">
<input type="hidden" name="perid" value="<?php echo $_REQUEST['perid']; ?>" />
<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
  <tr>
    <td class="subtitulodireita" width="50%">Atividade</td>
    <td>
    	<?php
    	$sql = "select 
					atpid as codigo,
					atpdsc as descricao 
				from maismedicos.pagamento_atividadeperiodo 
				where atpstatus = 'A'
		--and atpid = {$_REQUEST['atpid']}
				order by atpdsc";
    	
    	//$rs = $db->pegaLinha($sql);
    	//if($rs) echo '<font size="+1">'.$rs['descricao'].'</font>';
    	
    	$atpid = $_REQUEST['atpid'];
    	$db->monta_combo('atpid',$sql,$habilitado,'Selecione...',null,null,null,null,'S','atpid');
    	?>
    	<input type="hidden" name="atpid2" value="<?php echo $_REQUEST['atpid']; ?>" />
    </td>
  </tr>
  <!-- 
  <tr>
    <td class="subtitulodireita">Perfil</td>
    <td>
    	<?php
    	$sql = "select 
					pflcod as codigo,
					pfldsc as descricao 
				from seguranca.perfil 
				where pflstatus = 'A'
				and sisid = {$_SESSION['sisid']}
				order by pfldsc";
    	$db->monta_combo('pflcod',$sql,$habilitado,'Selecione...',null,null,null,null,'S','pflcod');
    	?>
    </td>
  </tr>
   -->
  <tr>
    <td class="subtitulodireita">Més de referência</td>
    <td>
    	<?php 
		$a=0;
		for($x=1;$x<13;$x++){
			$meses_periodo[$a]['codigo'] = str_pad($x,2,'0', STR_PAD_LEFT);
			$meses_periodo[$a]['descricao'] = mes_extenso($x);
			$a++;
		}
		$apgmes = date('m');
		$db->monta_combo('permes',$meses_periodo,'S','Mês',null,null,null,null,'N','permes');
		
		$b=0;
		for($y=date('Y')-1;$y<date('Y')+1;$y++){
			$anos_periodo[$b]['codigo'] = $y;
			$anos_periodo[$b]['descricao'] = $y;
			$b++;
		}
		$apgano = date('Y');
		$db->monta_combo('perano',$anos_periodo,'S','Ano',null,null,null,null,'N','perano');
		?>&nbsp;
    </td>
  <tr>
    <td class="subtitulodireita">Período para preenchimento ou execução das atividades</td>
    <td>
    <?php    
    echo campo_data2( 'perdtini', 'S', $habilitado, '', 'N','','' );
    echo ' até ';
    echo campo_data2( 'perdtfim', 'S', $habilitado, '', 'N','','' );
    ?>
    </td>
  <tr>
    <td class="subtitulodireita">Observação</td>
    <td><?php echo campo_textarea('perobs', '', $habilitado, '', $cols, $rows, $max)?></td>
  </tr>
  <tr>
    <td class="subtituloesquerda" colspan="2" align="center">
    <center>
    	<input type="button" value="Salvar" class="btnSalvar" />
    	<input type="button" value="Pesquisar" class="btnPesquisa" />
    	<input type="button" value="Limpar" class="btnLimpa" />
    	<input type="button" value="Voltar" class="btnVolta" />
    </center>
    </td>
  </tr>
</table>
</form>
<?php

$sql = "select 
		'<img src=\"../imagens/alterar.gif\" id=\"' || p.perid || '\" class=\"btnEditar\" />' as acao,
		permes || '/' || perano as ref,
		to_char(perdtini, 'DD/MM/YYYY') || ' até ' || to_char(perdtfim,'DD/MM/YYYY') as periodo,
		atpdsc
		from maismedicos.pagamento_atividadeperiodo a
		join maismedicos.pagamento_periodo p on p.atpid = a.atpid
		where atpstatus = 'A'
		and a.atpid = {$_REQUEST['atpid']}
		order by perano||permes desc";

$arrCabecalho = array('Ação', 'Mês', 'Período', 'Atividade');
$db->monta_lista($sql, $arrCabecalho, 100, 10, '', '');
?>

<?php else: ?>

<script>
$(function(){
	$('.btnEditar').click(function(){
		document.location.href = 'maismedicos.php?modulo=principal/pagamento/cadPeriodo&acao=A&atpid='+this.id;
	});
});
</script>

<?php

$sql = "select 
		'<img src=\"../imagens/alterar.gif\" id=\"' || a.atpid || '\" class=\"btnEditar\" />' as acao,
		atpdsc
		from maismedicos.pagamento_atividadeperiodo a
		where atpstatus = 'A'";

$arrCabecalho = array('Ação', 'Atividade');
$db->monta_lista($sql, $arrCabecalho, 100, 10, '', '');
?>

<?php endif; ?>

