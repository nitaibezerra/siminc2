<?php
	function filtrarNome($partipo = null){
		global $db;
		$partipo = !$partipo ? $_POST['partipo'] : $partipo;
		$sql = "
			select pes.pesid   as codigo,
			       pes.pesnome as descricao
			from atendimento.parlamentar par
				inner join atendimento.pessoa pes using(pesid)
			where pes.pesstatus = 'A'
		";
		if($partipo != '')
			$sql .= " and par.partipo = '".$partipo."'";
		$sql .= "
			order by 1
		";
		$db->monta_combo('pesid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'pesid');
	}
	
	if($_REQUEST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_REQUEST['requisicaoAjax']();
		exit;
	}
	
	if($_POST['acao'] == 'salvar')
		salvarBiografia($_POST);
	
	global $db;
	$pesid = !$pesid ? $_POST['pesid'] : $pesid;
	
	//Chamada de programa
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript">
	function filtrarNome(partipo){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=filtrarNome&partipo=" + partipo,
		   success: function(msg){
		   		$('#td_nome').html(msg);
		   	}
		 });
	}
	function validarFormulario(){
		if($('#acao').val() == 'consultar'){
			if($('#pesid').val() == ''){
				alert('Por favor selecione o parlamentar.');
				return false;
			}
		}
	}
</script>
<?php 	
	monta_titulo( 'Parlamentares', 'Biografia dos Parlamentares' );
	$permissao = permissoesPerfil();
?>
<form id="formulario" name="formulario" method="post" action="" onSubmit="return validarFormulario()">
	<input type="hidden" name="acao" id="acao" value="<?php if(!$pesid) echo 'consultar'; else echo 'salvar'; ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<?php 
			if(!$pesid){
		?>
				<tr>
					<td class="SubTituloDireita">Tipo:</td>
					<td>
						<input type="radio" name="partipo" id="partipo" value="D" onclick="filtrarNome(this.value)">
						Deputado Federal&nbsp;&nbsp;&nbsp;
						<input type="radio" name="partipo" id="partipo" value="S" onclick="filtrarNome(this.value)">
						Senador
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Nome:</td>
					<td id="td_nome">
						<?php 
							filtrarNome($partipo);
						?>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<input type="submit" name="Consultar" value="Consultar">
						<input type="reset" name="Cancelar" value="Cancelar" onclick="filtrarNome('')">
					</td>
				</tr>
		<?php 
			}
			else{
				$sql = "
					select pes.pesnome, 
						case 
						   when par.partipo = 'D' then 'Deputado Federal'
						   when par.partipo = 'S' then 'Senador'
					    end as partipo,
						pes.pestelefone,
						pes.pesemail,
						par.parpartido || '-' || par.estuf as partido,
						par.parid,
						bip.bipdescricao,
						bip.bipid
					from atendimento.parlamentar par
						inner join atendimento.pessoa               pes using(pesid)
						left  join atendimento.biografiaparlamentar bip using(parid)
					where pes.pesid   = ".$pesid;
				$dados = $db->pegaLinha($sql);
		?>
				<tr>
					<td class="SubTituloDireita">Nome:</td>
					<td>
						<?php echo $dados['pesnome']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Tipo:</td>
					<td>
						<?php echo $dados['partipo']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Telefone:</td>
					<td>
						<?php echo $dados['pestelefone']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">E-mail:</td>
					<td>
						<?php echo '<a href="mailto:'.$dados['pesemail'].'">'.$dados['pesemail'].'</a>'; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Partido - UF:</td>
					<td>
						<?php echo $dados['partido']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Biografia:</td>
					<td>
						<?php
							echo '<input type="hidden" name="parid" id="parid" value="'.$dados['parid'].'">';
							echo '<input type="hidden" name="bipid" id="bipid" value="'.$dados['bipid'].'">';
							echo campo_textarea('bipdescricao', 'S', 'S', '', '140', '30', '5000', '', 0, '', false, NULL, $dados['bipdescricao']);
						?>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<input type="submit" name="Salvar" value="Salvar">
						<input type="reset" name="Cancelar" value="Cancelar" onclick="location.href='atendimento.php?modulo=principal/biografia&acao=A'">
					</td>
				</tr>
		<?php 
			}			
		?>
	</table>
</form>