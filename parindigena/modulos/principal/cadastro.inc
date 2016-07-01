<?php

// Inclusão do arquivo de permissões (somente no módulo de obras)
if ($_SESSION["sisid"] == ID_OBRAS){
	require_once APPRAIZ . 'includes/cabecalho.inc';
	require_once APPRAIZ . "www/obras/permissoes.php";
}

// Inclusão de arquivos padrão do SIMEC
require_once APPRAIZ . 'includes/Agrupador.php';

// Inclusão de arquivos do componente de Entidade 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual   = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual   = substr($caminho_atual, 0 , $posicao_caminho);

// Executa as funções da tela de acordo com suas ações
switch ($_REQUEST["requisicao"]){
	case "cadastro":
		// Cadastra a obra
		obras_cadastra_obras($_REQUEST);
	break;
	case "atualiza":
		// Atualiza a obra
		obras_atualiza_obras($_REQUEST);
	break;
	default:
	break;
	
}

echo "<br/>";

// Se tiver a sessão, está em atualização de obra
if($_REQUEST["obrid"] || $_SESSION["obrid"]){
	
	if(!$_SESSION["obrid"]){
		$_SESSION["obrid"] = $_REQUEST["obrid"];
		$campus = '';
	}else{
		if($_REQUEST["obrid"]){
			unset($_SESSION['obrid'])
			$_SESSION["obrid"] = $_REQUEST["obrid"];
		}
	}
	
	$obrid = $_SESSION["obrid"];
	$dados_obra = obras_busca_obras($obrid);
	$requisicao = "atualiza";
	
	// Cria as abas do módulo
	$db->cria_aba($abacod_tela,$url,$parametros);
	
}else{
	$requisicao = "cadastro";
	$campus = 'display:none;';

}

// Cria o título da tela
$titulo_modulo = "Dados da Obra";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

?>
<script type="text/javascript">
	function RemoveLinha(index){
		table = window.document.getElementById("responsaveiscontato");
		table.deleteRow(index);
	}
</script>

<!-- INÍCIO DO FORMULÁRIO -->
<form id="formulario" name="formulario" method="post" onSubmit="return Validacao();" action="<?php echo $caminho_atual . "acao=A" ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Órgão:</td>
			<td>
				<?php
	
					// Exibe o orgão de acordo com a responsabilidade do usuário
					$orgid = $dados_obra["orgid"];
					$sql = "
							SELECT 
								orgid AS codigo, 
								orgdesc AS descricao 
							FROM 
								obras.orgao";
					
					$db->monta_combo("orgid", $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'S', 'orgid');
						
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade Implantadora:</td>
			<td>
				<?php 
	
					// Cria o objeto com da entidade
					$entidade = new Entidade($dados_obra["entidunidade"]);
					$entnome = $entidade->entnome;
					$entid   = $entidade->getPrimaryKey();
					
				?>
	
  				<span id="entnome"><?php echo $entnome; ?></span>
  				<input type="hidden" name="entid" id="entid" value="<? if(isset($_SESSION["obrid"])){ echo $entid; } ?>">
				<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirEntidade(document.getElementById('entid').value,document.getElementById('orgid').value);" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
				<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
		</tr>
		<tr id="campus" style="<?php echo $campus; ?>">
			<td class="SubTituloDireita">Campus Universitário:</td>
			<td id="mostracampus">
				<?php
					
					$campus = new Entidade($dados_obra["entidcampus"]);
					$campusnome = $campus->entnome;
					$entidcampus = $campus->getPrimaryKey();
					
					if ($_SESSION["obrid"]){
						$sql = "
							SELECT 
								entid as codigo,
								entnome as descricao 
							FROM 
								entidade.entidade
							WHERE
								entidassociado = {$entid} AND
								funid = 18";
							$db->monta_combo("entidcampus", $sql, "S", "Selecione...", '', '', '', '', 'N', 'entidcampus');	
					}
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Local da Obra:</td>
			<td>
				<?php $obrdescundimplantada = $dados_obra["obrdescundimplantada"]; ?>
				<?= campo_texto( 'obrdescundimplantada', 'S', $somenteLeitura, '', 60, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome da Obra:</td>
			<td>
				<?php $obrdesc = $dados_obra["obrdesc"]; ?>
				<?= campo_texto( 'obrdesc', 'S', $somenteLeitura, '', 60, 60, '', '', 'left', '', 0); ?>
			</td>
		</tr>
		
		<!-- INÍCIO DO COMPONENTE DE CEP -->
		<tr>
			<td style="font-weight: bold" colspan="2">Endereço</td>
		</tr>
        <tr>
        	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
			<td>
				<?php

					$endereco = new Endereco($dados_obra["endid"]);
				
				?>
                <input type="text" name="endcep" onkeyup="this.value=mascaraglobal('##.###-###', this.value);" onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo" id="endcep" value="<?php echo $endereco->endcep; ?>" size="13" maxlength="10" />
                <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
		</tr>
		<tr id="escolha_logradouro_id" style="display:none">
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Selecione o Logradouro:</label></td>
            <td>
				<input readonly="readonly" type="text" name="endlog" class="CampoEstilo" id="endlog" value="<?php echo $endereco->endlog; ?>" size="48" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>
			<td>
				<input readonly="readonly" type="text" name="endlog" class="CampoEstilo" id="endlogradouro" value="<?php echo $endereco->endlog; ?>" size="48" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
			<td>
				<input type="text" name="endnum" class="CampoEstilo" id="endnum" value="<?php echo $endereco->endnum; ?>" size="5" maxlength="8" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
			<td>
				<input type="text" name="endcom" class="CampoEstilo" id="endcom" value="<?php echo $endereco->endcom; ?>" size="48" maxlength="100" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
			<td>
				<input readonly="readonly" type="text" name="endbai" class="CampoEstilo" id="endbai" value="<?php echo $endereco->endbai; ?>" />
			</td>
		</tr>
        <tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>
			<td>
				<?php 
				
					if ($_SESSION["obrid"]){
						$sql = "";
						$mundescricao = $db->pegaUm("
												SELECT 
													mundsc 
												FROM 
													municipio 
												WHERE 
													muncod = {$endereco->muncod}");
					}
					
				?>
				<input readonly="readonly" type="text" name="mundescricao" class="CampoEstilo" id="mundescricao" value="<?php echo $mundescricao; ?>" />
				<input type="hidden" name="muncod" id="muncod" class="CampoEstilo" value="<?php echo $endereco->muncod; ?>" />
				<input readonly="readonly" type="text" name="estuf" class="CampoEstilo" id="estuf" value="<?php echo $endereco->estuf; ?>" style="width: 5ex; padding-left: 2px" />
			</td>
		</tr>
		<tr>
			<td style="font-weight: bold" colspan="2">Coordenadas Geográficas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Latitude</td>
			<td>
				<?php

					// Separa os dados da coordenada 
					$medlatitude = $endereco->medlatitude;
					$latitude = explode(".", $medlatitude);
					$graulatitude = $latitude[0];
					$minlatitude = $latitude[1];
					$seglatitude = $latitude[2];
					$pololatitude = $latitude[3];
				?>
				<?= campo_texto( 'graulatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?> °
				<?= campo_texto( 'minlatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?> '
				<?= campo_texto( 'seglatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?> ''
				<?= campo_texto( 'pololatitude', 'N', $somenteLeitura, '', 1, 1, '', '', 'left', '', 0, 'id="pololatitude"'); ?> (N/S)
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude</td>
			<td>
				<?php

					// Separa os dados da coordenada 
					$medlongitude = $endereco->medlongitude;
					$longitude = explode(".", $medlongitude);
					$graulongitude = $longitude[0];
					$minlongitude = $longitude[1];
					$seglongitude = $longitude[2];
				?>
				<?= campo_texto( 'graulongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?> °
				<?= campo_texto( 'minlongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?> '
				<?= campo_texto( 'seglongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"'); ?> '' &nbsp W
			</td>
		</tr>
			<?php

				// Se os dados das coordenadas estiverem preenchidos, exibe o link para o mapa
				if($graulongitude != "" && $minlongitude != "" && $seglongitude != "" && $graulatitude != "" && $minlatitude != "" && $seglatitude != ""){ 
			?>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
				<a href="#" onclick="abreMapa();">Visualizar No Mapa</a>
			</td>
		</tr>
		<?php } ?>
		<tr>
			<td style="font-weight: bold" colspan="2">Sobre a Obra</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Obra:</td>
			<td>
				<?php
	
					$tobraid = $dados_obra["tobraid"];
					$sql = "
						SELECT 
							tobaid AS codigo, 
							tobadesc AS descricao 
						FROM 
							obras.tipoobra";
					
					$db->monta_combo("tobraid", $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'N'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação da Obra:</td>
			<td>
				<?php	
				
					$stoid = $dados_obra["stoid"];
					$sql = "
						SELECT 
							stoid AS codigo, 
							stodesc AS descricao 
						FROM 
							obras.situacaoobra";
					
					$db->monta_combo("stoid", $sql, $somenteLeitura, "Selecione...", '', '', '', '175', 'S'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Início programado para:</td>
			<td>
				<?php $obrdtinicio = $dados_obra["obrdtinicio"]; ?>
				<?= campo_data( 'obrdtinicio', 'N', $somenteLeitura, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Término programado para:</td>
			<td>
				<?php $obrdttermino = $dados_obra["obrdttermino"]; ?>
				<?= campo_data( 'obrdttermino', 'N', $somenteLeitura, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Custo do Contrato R$:</td>
			<td>
				<?php 
					
					$obrcustocontrato = $dados_obra["obrcustocontrato"];
					$_SESSION["obrcustocontrato"] = $obrcustocontrato;
					$obrcustocontrato = number_format($obrcustocontrato,2,',','.');
					
				?>
				<?= campo_texto( 'obrcustocontrato', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'left', '', 0, 'id="obrcustocontrato" onblur="preencheValor();"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Área/Quantidade Construída:</td>
			<td>
				<?php 
					$obrqtdconstruida = $dados_obra["obrqtdconstruida"];
					$obrqtdconstruida = number_format($obrqtdconstruida,2,',','.'); 
				?>
				<?= campo_texto( 'obrqtdconstruida', 'N', $somenteLeitura, '', 17, 13, '##.###.###,##', '', 'left', '', 0, 'id="obrqtdconstruida" onblur="preencheValor();"');?> 
				Unidade de Medida: 
				<?php					
					$sql = "
						SELECT 
							umdid AS codigo, 
							umdeesc AS descricao 
						FROM 
							obras.unidademedida";
					$db->monta_combo("umdidobraconstruida", $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'N');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Custo Unitário R$:</td>
			<td>
				<?php 
					$obrcustounitqtdconstruida = $dados_obra["obrcustounitqtdconstruida"];
					$obrcustounitqtdconstruida = number_format($obrcustounitqtdconstruida,2,',','.'); 
				?>
				<?= campo_texto( 'obrcustounitqtdconstruida', 'N', 'N', '', 17, 20, '###.###.###,##', '', 'left', '', 0, 'id="obrcustounitqtdconstruida"'); ?>
				(R$ / Unidade de Medida)
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Percentual BDI:</td>
			<td>
				<?= campo_texto( 'obrpercbdi', 'N', $somenteLeitura, '', 17, 20, '###,##', '', 'left', '', 0, 'id="obrpercbdi"'); ?>
				(Administração, taxas, emolumentos, impostos e lucro.)
			</td>
		</tr>
		<?php
			// Se for uma obra indígena exibe os campos 
			if($_SESSION["sisid"] == ID_PARINDIGENA){ 
		?>
		<tr>
			<td style="font-weight: bold" colspan="2">PAR Indígena</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Povos Atendidos</td>
			<td>
				<?php 

					$sql = "
						SELECT
							povid AS codigo,
							povnome AS descricao
						FROM
							parindigena.povoindigena";
					
						combo_popup( "povid", $sql, "Selecione o(s) Povo(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 8, 310 );
	 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Territórios Atendidos</td>
			<td>
				<?php 

					$sql = "
						SELECT
							arrid AS codigo,
							arrnome AS descricao
						FROM
							parindigena.territorioetnoeducacional";
				
					combo_popup( "arrid", $sql, "Selecione o(s) Território(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 8, 310 );
	 
				?>
			</td>
		</tr>
		<?php } ?>
		<tr>
			<td style="font-weight: bold" colspan="2">Contratação da Obra</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Empresa Contratada</td>
			<td>
				<?php 
					$empresa = new Entidade($dados_obra["entidempresaconstrutora"]);
					$entnomeempresa = $empresa->entnome;
					$entidempresa = $empresa->getPrimaryKey();
				?>
  				<span id="entnomeempresa"><?php echo $entnomeempresa; ?></span>
  				<input type="hidden" name="entidempresa" id="entidempresa" value="<? if(isset($obrid)) echo $entidempresa; ?>">
  				<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirEmpresa(document.getElementById('entidempresa').value);" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
			</td>
		</tr>
		<tr>
			<td style="font-weight: bold" colspan="2">Contatos</td>
		</tr>
		<tr>
			<td colspan="2">
				<table id="responsaveiscontato" width="75%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<thead>
						<tr id="cabecalho">
							<td width="15%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
							<td width="55%" valign="top" align="center" class="title" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
							<td width="30%" valign="top" align="center" class="title" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Categoria de Responsabilidade</strong></td>
						</tr>
					</thead>
					<?php 

						// Cria a lista com os responsáveis caso esta na atualização
						if($_SESSION["obrid"]){
							
							$sql = pg_query("
									SELECT
										rc.tprcid as tipo, 
										rc.recoid as responsavel,
										rc.entid as entidade, 
										et.entnome as nome  
									FROM 
										obras.responsavelobra r 
									INNER JOIN 
										obras.responsavelcontatos rc ON r.recoid = rc.recoid
									INNER JOIN 
										entidade.entidade et ON rc.entid = et.entid
									WHERE 
										r.obrid = '". $_SESSION['obrid'] . "'  AND rc.recostatus = 'A'");
							
							while (($dados = pg_fetch_assoc($sql))){
						 		$tipo = $dados["tipo"];
						 		$responsavel = $dados['responsavel'];
						 		$entidade = $dados['entidade'];
						 		$id = $dados['recoid'];
						 		$nome = $dados['nome'];
						 		
								$sql2 = pg_query("
									SELECT 
										tiporesp.tprcid, 
										tiporesp.tprcdesc
									FROM 
										obras.tiporespcontato AS tiporesp");
								
								if($somenteLeitura == "S")
									$combo_responsabilidade = "<select style='width:170px' class='CampoEstilo' name=\"tprcid[" . $entidade . "]\">";
								else
									$combo_responsabilidade = "<select style='width:170px' class='CampoEstilo' name=\"tprcid[" . $entidade . "]\" disabled>";
								
								while ($tipos = pg_fetch_array($sql2)){
									
									$selected = "";
									$tprcid = $tipos['tprcid'];
							        $tprcdescricao = $tipos['tprcdesc'];
									
							        if ($tipo == $tprcid){
										$selected = "selected";
										
									}
							        $combo_responsabilidade .= "<option value='{$tprcid}' {$selected}>" . $tprcdescricao . "</option>";
								}
							    
								$combo_responsabilidade .= "</select>";
								
								if($somenteLeitura == "S")
							    	$botoes = "<img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" border=0 title=\"Editar\" onclick=\"atualizaResponsavel('" . $entidade . "')\">&nbsp&nbsp&nbsp<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\" onClick=\"RemoveLinha(window.document.getElementById('linha_".$entidade."').rowIndex);\">";
							    else
									$botoes = "<img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" border=0 title=\"Editar\">&nbsp&nbsp&nbsp<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\">";
							    
								echo "
									<tr id=\"linha_" . $entidade . "\">
										<td bgcolor=\"#F7F7F7\" align=\"center\">" . $botoes . "</td>
										<td>" . $codigo . $nome . $id . "</td>
										<td>" . $combo_responsabilidade . "</td>
									</tr>";
							}
						}
					
					?>	
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<?php if($somenteLeitura=="S") { ?>
					<a href="#" onclick="inserirResponsavel(); return false;"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Contatos"> Inserir Contatos</a>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Observação sobre a Obra:</td>
			<td>
				<?php $obsobra = $dados_obra["obsobra"]; ?>
				<?= campo_textarea( 'obsobra', 'N', $somenteLeitura, '', '70', '4', '500'); ?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<input type="hidden" name="requisicao" value="<?php echo $requisicao; ?>" />
					<input type="submit" value="Salvar" style="cursor: pointer" <?php if($somenteLeitura=="N") echo "disabled"; ?>> 
							<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
						</div>
					</td>
				</tr>
			</table>
		</form>