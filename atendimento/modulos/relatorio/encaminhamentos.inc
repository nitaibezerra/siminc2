<?php 
	include APPRAIZ . 'includes/cabecalho.inc';
	echo '<br/>';
	monta_titulo( 'Relatório de Eventos', '' );
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript">
	function relatorio(evtid){
		var form = document.formulario;
		$('#evtid').val(evtid);
		$('#visualizar').val('relatorio');
		form.submit();
	}
	
	function pesquisar(){
		if(jQuery('#evtdataini').val()=='') {
			alert('Preencha data de início');
			return false;
		}
		if(jQuery('#evtdatafim').val()=='') {
			alert('Preencha data de fim');
			return false;
		}		
		var form = document.formulario;
		$('#visualizar').val('pesquisa');
		prepara_formulario();
		form.submit();
	}
</script>
<form name="formulario" id="pesquisar" method="POST" action="">
	<input type="hidden" name="evtid" id="evtid" value="">
	<input type="hidden" name="visualizar" id="visualizar" value="pesquisa"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data Inicial:</td>
			<td>
				<?php echo campo_data('evtdataini','S','S','','S','','',$POST['evtdataini']); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data Final:</td>
			<td>
				<?php echo campo_data('evtdatafim', 'N', 'S', '', 'S',$POST['evtdatafim']); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input style="cursor:pointer;" type="button" value="Pesquisar" onClick="pesquisar()" />
			</td>
		</tr>
	</table>
</form>
<?php 
	if(!empty($_POST['evtdataini']) && !empty($_POST['evtdatafim'])){
		$evtdataini = formata_data_sql($_POST['evtdataini']);
		$evtdatafim = formata_data_sql($_POST['evtdatafim']);
		$sql = "SELECT pesnome, pesemail, ecmdescricao, to_char(ecmprazo,'DD/MM/YYYY') as ecmprazo
				FROM atendimento.encaminhamento
				INNER JOIN atendimento.pevencaminhamento USING(ecmid)
				INNER JOIN atendimento.pessoaevento USING(pevid)
				INNER JOIN atendimento.pessoa USING(pesid)
				WHERE ecmstatus = 'A'
				AND tppid = 1
				AND ecmprazo BETWEEN '".$evtdataini."' AND '".$evtdatafim."'";
	} else {
		$sql = "SELECT pesnome, pesemail, ecmdescricao, to_char(ecmprazo,'DD/MM/YYYY') as ecmprazo 
				FROM atendimento.encaminhamento
				INNER JOIN atendimento.pevencaminhamento using(ecmid)
				INNER JOIN atendimento.pessoaevento using(pevid)
				INNER JOIN atendimento.pessoa using(pesid)
				WHERE ecmstatus = 'A'
				AND tppid = 1";
	}
	
	$cabecalho = array('Nome', 'E-mail', 'Descrição', 'Prazo');
	$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '95%');	
?>


