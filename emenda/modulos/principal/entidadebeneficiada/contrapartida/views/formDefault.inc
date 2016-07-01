<style>
	#div_rolagem table {
		width: 100%;                
	}
</style>

<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
function carregaModalidade(resid){
	document.getElementById('requisicao').value = 'mod';
	document.getElementById('formulario').submit();
}

function salvarModalidade(){
	document.getElementById('requisicao').value = 'modalidade';
	//tercontrapartida_
	var bovazio = 'false';
	if( $('resid').getValue() == '' ){
		alert('O campo Responsavél é de preenchimento obrigatório!');
		$('resid').focus();
		return false;
	} else {
		for(i=0; i<formulario.length; i++){
			//alert(formulario.elements[i].id);
			if(formulario.elements[i].id.indexOf('rmccontrapartida_') != -1 ){
				if( formulario.elements[i].value != '' ){
					bovazio = 'true';
				}
			}			
		}
		
		if( bovazio == 'false' ){
			alert('É necessário informar pelo uma porcentagem para modalidade de aplicação!');
			return false;
		}
	}
	document.getElementById('formulario').submit();
}
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="pesquisa" name="requisicao" id="requisicao"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="2" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td width="190px" class="subtitulodireita">Exercício:</td>
			<td>
				<?php echo $_SESSION["exercicio"]; ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">CNPJ:</td>
			<td>
				<?php
					$enbcnpj = $_REQUEST["enbcnpj"]; 
					echo campo_texto('enbcnpj', 'N', 'S', '', 22, 18, '##.###.###/####-##', '', 'left', '', 0, 'id="enbcnpj"', '', $enbcnpj);  
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nome Entidade:</td>
			<td>
				<?php
					
					$entnome = $_REQUEST["enbnome"];
					echo campo_texto( 'enbnome', 'N', 'S', '', 60, 150, '', '', 'left', '', 0, 'id="enbnome"', '', $entnome);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Contrapartida Mínima Definida:</td>
			<td>
				<input type="radio" value="S" id="contrapartida" name="contrapartida" <?=$_REQUEST["contrapartida"] == 'S' ? 'checked=checked' : '' ?> /> Sim
				<input type="radio" value="N" id="contrapartida" name="contrapartida" <?=$_REQUEST["contrapartida"] == 'N' ? 'checked=checked' : '' ?> /> Não
				<input type="radio" value=""  id="contrapartida" name="contrapartida" <?=$_REQUEST["contrapartida"] == ''  ? 'checked=checked' : '' ?> /> Todas
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();" value="Pesquisar"/>
			</td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: left;"><b>Definir Contrapartida por Modalidade de Aplicação</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Responsavél:</td>
			<td><?
				$resid = $_POST['resid'];
				$sql = "SELECT resid as codigo, resdsc as descricao FROM emenda.responsavel WHERE resstatus = 'A'";
				$db->monta_combo("resid", $sql, 'S', 'Selecione...', 'carregaModalidade', '', '', '200', 'S', 'resid' ); ?></td>
		</tr>
		<tr>
			<td colspan="2"><table class="tabela" bgcolor="#f5f5f5" style="width: 40%" cellSpacing="1" cellPadding=3 align="left">
				<tr>
					<th>Modalidade de Aplicação</th>
					<th>Contrapartida Mínima</th>
				</tr>
				<?
				foreach ($arTipos as $key => $v) {
					echo '<tr>
							<td class="subtitulodireita">'.$v['mapdsc'].':</td>
							<td>'.campo_texto('rmccontrapartida['.$v['mapcod'].']', 'N', 'S', '', 8, 6, '###,##', '', 'left', '', 0, 'id="rmccontrapartida_'.$v['mapcod'].'"', '', $v['rmccontrapartida']).' %</td>
						</tr>';
				}
				
				?>
				<tr bgcolor="#D0D0D0">
					<td colspan="2" style="text-align: center;">
						<input type="button" style="cursor: pointer;" onclick="salvarModalidade();" value="Salvar Modalidade"/>
					</td>
				</tr>
			</table></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: left;"><b>Definir Contrapartida por Entidade Beneficiada</b></td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: center;"><b>Lista de Entidades Beneficiadas</b></td>
		</tr>
	</table>
</form>

<center>
	<div id="div_rolagem" style="overflow-x: auto; overflow-y: auto; width:95%; height:300px;">
		<form id="forContra" name="forContra" method="post" action="">
			<input type="hidden" value="cadastra" name="requisicao"/>
			<?php $obEntContra->listaEntidadesContrapartida( $filtros ); ?>
			
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#D0D0D0">
					<td>
						<input type="button" value="Salvar Contrapartida" onclick="document.getElementById('forContra').submit();" style="cursor: pointer;" <?=disabled();?> />
						<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
	</div>
</center>