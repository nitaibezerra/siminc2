<?php
if($_REQUEST['tpeid'])
	$_SESSION['sig_var']['tpeid'] = $_REQUEST['tpeid'];

$funid = $_tipoentidade[$_SESSION['sig_var']['tpeid']];
 
if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}

switch($_POST['alterabd']) {
	case 'inserir':
		// Inserindo campus
		$unidados = $_REQUEST['unidade'];
		/* Ajustando datas (mm/yyyy)
	 	 * Solicitado pelo Hugo (devido ordenação)
	 	 * formato de saída : yyyymm 
	 	 */
		$_POST['cmpdataimplantacao'] = substr($_POST['cmpdataimplantacao'],3,4).substr($_POST['cmpdataimplantacao'],0,2);
		//$_POST['cmpdatainicioaula'] = substr($_POST['cmpdatainicioaula'],3,4).substr($_POST['cmpdatainicioaula'],0,2);
		
		
		$sql = "INSERT INTO sig.campus(
            	entid, cmpobs, cmpdataatualizacao, usucpf, cmpdataimplantacao, 
            cmpdatainauguracao, cmpexistencia, cmpsituacao, 
            cmpinstalacao, cmpsituacaoobra)            	
    			VALUES ('". $_POST['cmpid'] ."', 
    					'". substr($_POST['cmpobs'],0,1000) ."',
    					NOW(),
    					'".$_SESSION['usucpf']."',
    					'".trim($_POST['cmpdataimplantacao'])."', 
    					".(($_POST['cmpdatainauguracao'])?"'".formata_data_sql($_POST['cmpdatainauguracao'])."'":'NULL').",	'".$_POST['cmpexistencia']."',
    					'".$_POST['cmpsituacao']."','".$_POST['cmpinstalacao']."',
    					'".$_POST['cmpsituacaoobra']."') 
    			RETURNING cmpid;";	
		$cmpid = $db->pegaUm($sql);
		$db->commit();
		echo "<script>
				alert('Os dados do campus foram cadastrados com sucesso.');
				window.location = '?modulo=inicio&acao=C&tpeid=".$_REQUEST['tpeid']."';
			  </script>";
		exit;
		break;
}
session_start();
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
echo "<br/>";

// Verificando segurança
$permissoes = verificaPerfilSig();

if(!$_REQUEST['tpeid'] && count($permissoes['vertipoensino']) > 0) {
	$_REQUEST['tpeid'] = current($permissoes['vertipoensino']);

}
if($permissoes['vertipoensino'][0]) {
	validaAcessoTipoEnsino($permissoes['vertipoensino'],$_REQUEST['tpeid']);
}
// Fim segurança

// Monta menu contendo os tipos de ensino (DE ACORDO COM O PERFIL)
if($permissoes['vertipoensino'][0]) {
	$sql = "SELECT tpeid AS id, tpedsc AS descricao, '/sig/sig.php?modulo=principal/cadastrarcampus&acao=A&tpeid=' || tpeid AS link  
			FROM sig.tipoensino 
			WHERE tpeid IN('".implode("','",$permissoes['vertipoensino'])."')";
	$menutipoensino = $db->carregar($sql);
	if ($menutipoensino){
   		echo montarAbasArray($menutipoensino, '/sig/sig.php?modulo=principal/cadastrarcampus&acao=A&tpeid='.$_REQUEST['tpeid']);
	}

$titulo_modulo = "Sistema de Informações Gerenciais";
monta_titulo( $titulo_modulo, 'Cadastrar novo campus' );
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/sig.js"></script>
<form id="formulario" name="formulario" method="post" action="?modulo=principal/cadastrarcampus&acao=A">
<input type="hidden" name="tpeid" value="<? echo $_REQUEST['tpeid']; ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Tipo de ensino :</td>
			<td>
			<? 
			if($menutipoensino) {
				foreach($menutipoensino as $tipo) {
					if($tipo['id'] == $_REQUEST['tpeid']) {
						echo $tipo['descricao'];
					}
				}
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Instituição :</td>
			<td>
			<?			
			echo "<select class='CampoEstilo' style='width: auto' name='unidade' id='unidades' onchange=\"listarCampusCadastro(this.value, ". ((is_array($funid))?"new Array(".implode(",",$funid).")":$funid)." );\">";
			
			if($_REQUEST['tpeid']) {
				if(count($permissoes['verunidade'][$_REQUEST['tpeid']]) > 0) {
					$filtroentidade = "AND entid IN('".implode("','",$permissoes['verunidade'][$_REQUEST['tpeid']])."')";
				}
				
				$sql = "SELECT entid, entnome
						FROM entidade.entidade
						WHERE funid IN('".((is_array($funid))?implode("','",$funid):$funid)."') ".$filtroentidade."
						ORDER BY entnome";				
				$unidades = $db->carregar($sql);
				
				if($unidades) {
					echo "<option value=''>Selecione</option>";
					foreach($unidades as $unidade) {
						echo "<option value='". $unidade['entid']."'>". $unidade['entnome'] ."</option>";
					}
				}
			} else {
				echo "<option value=''>Selecione</option>";
			}
			echo "</select>";
			?>
			<img src="../imagens/obrig.gif" border="0">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Nome do Campus :</td>
			<td id="nome_campus">			
			<?			
			echo "<select disabled='disabled' id='cmpid' class='CampoEstilo' style='width: auto' name='cmpid' >";
			echo "<option value=''>Selecione</option>";
			echo "</select>";
			
			?>
			<img src="../imagens/obrig.gif" border="0">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF :</td>			
			<td>
			<? 
			
			echo campo_texto('estuf', "N", "N", "Nome do campus", 40, 100, "", "", '', '', 0, '' );
						
		 	?>			
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Município :</td>
			<td>
			<?		
			
			echo campo_texto('muncod', "N", "N", "Nome do campus", 40, 100, "", "", '', '', 0, '' );
			
			?>			
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Observações :</td>
			<td><? echo campo_textarea( 'cmpobs', 'N', 'S', '', '70', '4', '1000'); ?></td>
		</tr>
		<!--<tr>
			<td class="SubTituloDireita">Data da aula inaugural :</td>
			<td><? echo campo_texto('cmpdatainicioaula', "N", "S", "Data de Início das Aulas", 8, 7, "##/####", "", '', '', 0, '' ); ?></td>
		</tr>-->
		<tr>
			<td class="SubTituloDireita">Data de implantação :</td>
			<td><? echo campo_texto('cmpdataimplantacao', "N", "S", "Data de Implantação", 8, 7, "##/####", "", '', '', 0, '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de inauguração :</td>
			<td><? echo campo_texto('cmpdatainauguracao', "N", "S", "Data de inauguração", 12, 10, "##/##/####", "", '', '', 0, '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Existência do campus/uned :</td>
			<td><input type='radio' name='cmpexistencia' value='N' <? echo (($dadoscampus['cmpexistencia']=='N')?'checked':''); ?>> Novo <input type='radio' name='cmpexistencia' value='P' <? echo (($dadoscampus['cmpexistencia']=='P')?'checked':''); ?>> Pré-Existente</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação do campus/uned :</td>
			<td><input type='radio' name='cmpsituacao' value='F' <? echo (($dadoscampus['cmpsituacao']=='F')?'checked':''); ?> onclick='verificasituacaocampus(this);'> Funcionando <input type='radio' name='cmpsituacao' value='N' <? echo (($dadoscampus['cmpsituacao']=='N')?'checked':''); ?> onclick='verificasituacaocampus(this);'> Não Funcionando</td>
		</tr>
		<tr id='trcmpinstalacao' <? echo (($dadoscampus['cmpsituacao']=='F')?'':'style="display:none"'); ?>>
			<td class="SubTituloDireita">Instalações :</td>
			<td><input type='radio' id='cmpinstalacaoP' name='cmpinstalacao' value='P' ".(($dadoscampus['cmpinstalacao']=='P')?'checked':'')."> Instalações Provisórias <input type='radio' id='cmpinstalacaoD' name='cmpinstalacao' value='D' <? echo (($dadoscampus['cmpinstalacao']=='D')?'checked':''); ?>> Instalações Definitivas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação da obra no campus/uned :</td><td>
			<?
			$db->monta_combo('cmpsituacaoobra', array(0 =>array("codigo" => "L", "descricao" => "Licitação de Obras"),1 =>array("codigo" => "A", "descricao" => "Obras em Andamento"),2 =>array("codigo" => "C", "descricao" => "Obras Concluídas")), 'S', 'Selecione', '', '', '', '200', 'N', 'cmpsituacaoobra');
			?></td>
		</tr>
		<? if($_REQUEST['tpeid'] == TPENSSUP) { ?>
				<!--<tr>
					<td class="SubTituloDireita">Campus faz parte da 1ª etapa :</td><td>
					<?
						$db->monta_combo('cmp1etapa', array(0 =>array("codigo" => 1, "descricao" => "Não se aplica"),1 =>array("codigo" => 2, "descricao" => "Em Andamento"),2 =>array("codigo" => 3, "descricao" => "Concluída")), 'S', 'Selecione', '', '', '', '200', 'N', 'cmp1etapa');
				 	?>
					</td></tr> -->
		<? } ?>
<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<input type="hidden" name="alterabd" value="inserir" />
					<input type="button" value="Salvar" onclick="validarFormularioCadastrarCampus();" style="cursor: pointer" <?php if($somenteLeitura=="N") echo "disabled"; ?>> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="this.disabled=true;window.location = '?modulo=inicio&acao=C&tpeid=<? echo $_REQUEST['tpeid']; ?>'">
				</div>
			</td>
		</tr>
	</table>
</form>
<?
} else {
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr bgcolor="red">
		<td colspan="2" align="center">NÃO FOI ATRIBUÍDO NENHUM <strong>TIPO DE ENSINO</strong> AO SEU PERFIL. ENTRE EM CONTATO COM ADMINISTRADOR DO SISTEMA.</td>
	</tr>
	</table>
<?
}
?>