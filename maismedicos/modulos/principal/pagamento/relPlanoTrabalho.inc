<?php 

if($_REQUEST['anexo']=='download'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die;
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

include_once APPRAIZ . 'maismedicos/classes/Planotrabalho.class.inc';
$obPt = new Planotrabalho();

monta_titulo('Monitoramento dos Planos de Trabalho', 'Selecione os filtros abaixo');
$db->cria_aba( $abacod_tela, $url, $parametros);
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
<script type="text/javascript">
$(function(){
	$('.btnLimpar').click(function(){
		document.location.href = 'maismedicos.php?modulo=principal/pagamento/relPlanoTrabalho&acao=A';
	});
	$('.btnDetalhe').click(function(){
		params = this.id.split('_');
		cpf = params[0];
		semestre = params[1];
		url = 'maismedicos.php?modulo=principal/pagamento/relPlanoTrabalho&acao=A';
		url += '&requisicao=montaDetalhePlanoTrabalho&bencpf='+cpf+'&bensemestre='+semestre
		document.location.href = url;
	});
	$('.btnDownloadAnexo').click(function(){
		url = document.location.href+'&anexo=download&arqid='+this.id;
		var popUp = window.open(url, 'popDownloadAnexo', 'height=500,width=400,scrollbars=yes,top=50,left=200');
		popUp.focus();
	});
});

</script>

<?php 
if($_REQUEST['requisicao']){

	print $obPt->$_REQUEST['requisicao']($_REQUEST);
	echo '<center><p><button onclick="javascript:history.go(-1)">Voltar</button></p></center>';

}else{
?>
<form name="formulario" method="post" action="">
<table class="tabela" cellpadding="3" cellspacing="1" widht="95%" align="center">
	<tr>
		<td class="subtitulodireita" width="40%">Semestre</td>
		<td>
		<?php 
		$sql = "select distinct 
					ptrano::varchar || '_' || ptrsemestre::varchar as codigo,
					ptrsemestre::varchar || '/' || ptrano::varchar as descricao					
				from maismedicos.planotrabalho 
				where ptrstatus = 'A' order by ptrano::varchar || '_' || ptrsemestre::varchar desc ";
		
		$db->monta_combo('semestre',$sql,'S','Semestre',null,null,null,null,'N','semestre');
		?>
		</td>
	</tr>
		
	<tr>
		<td class="subtitulodireita">Beneficiário</td>
		<td>
		<?php
		
		$sql = "select
	 				bencpf as codigo,
	 				bennome as descricao
	 			from maismedicos.pagamento_beneficiario
	 			where benstatus = 'A'
	 			order by removeacento(lower(trim(bennome))) asc";
		
		$db->monta_combo('beneficiario',$sql,'S','Beneficiário',null,null,null,null,'N','beneficiario');
		?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Período</td>
		<td>
			<?php echo campo_data2( 'perini', 'N', 'S', '', 'N','','' );?>
                	&nbsp;até&nbsp;
                	<?php echo campo_data2( 'perfim', 'N', 'S', '', 'N','','' );?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"></td>
		<td class="subtituloesquerda">
			<input type="submit" value="Pesquisar" class="btnPesquisar" />
			<input type="button" value="Limpar" class="btnLimpar" />
		</td>
	</tr>
</table>
</form>
<?php 
$arDados = $obPt->recuperaPlanosBeneficiarios($_POST);
$arDados = $arDados ? $arDados : array();
$arrCabecalho = array('Ação', 'CPF', 'Nome','Semestre',  'Visitas', 'Atividades');
$db->monta_lista($arDados,$arrCabecalho,50,10,'','','');
}
?>