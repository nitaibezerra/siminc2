<?

function limparEstado(){
	
	if($_REQUEST['estuf']) {
		$sql = "DELETE FROM cte.relatoriogeralfinanceiro WHERE rgfuf='".$_REQUEST['estuf']."'";
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Dados removidos com sucesso');
			window.location.href = window.location.href;
		 </script>");
	
}

function limparCarga(){
	
	if($_REQUEST['datetime']) {
		$sql = "DELETE FROM cte.relatoriogeralfinanceiro WHERE rgfdatains='".$_REQUEST['datetime']."'";
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Dados removidos com sucesso');
			window.location.href = window.location.href;
		 </script>");
	
}

function enviarArquivo(){
	
	global $db;
	
	$data_ins = date("Y-m-d h:i:s");

// 	ver($_FILES, $_POST,d);
	$files = $_FILES;
	
	if( (count($_POST['estuf'][1]) > 0 && $_FILES['arquivo_1']['error'] == 0) || $_FILES['arquivo_0']['error'] == 0  ){
		
	    $sql = "INSERT INTO obras.pretipoobra(
			            ptodescricao, ptostatus, ptoclassificacaoobra, ptopercentualempenho, tooid, ptoprojetofnde, ptoesfera, ptoexisteescola, tpoid, pcoid, ptoplanilhaporregiao, ptopreencher)
			    VALUES ('{$_POST['ptodescricao']}', 'A', '{$_POST['ptoclassificacaoobra']}', {$_POST['ptopercentualempenho']}, {$_POST['tooid']},
			    		'{$_POST['ptoprojetofnde']}', '{$_POST['ptoesfera']}','t', {$_POST['tpoid']}, {$_POST['pcoid']}, '{$_POST['ptoplanilhaporregiao']}', {$_POST['ptopreencher']} )
			    RETURNING ptoid";
	
	    $ptoid = $db->pegaUm( $sql );
	    $db->commit();
	    
	    if( $_POST['ptoplanilhaporregiao'] == 'N' ){
	    	$_POST['estuf'] = Array(Array('nacional'));
	    }
	}
	
// 	ver($_POST['estuf']);
    
	if($ptoid) {
// 		ver($_FILES,d);
		foreach( $_POST['estuf'] as $k => $ufs ){
			
			unset($dados);
			$dados = file($_FILES['arquivo_'.$k]['tmp_name']);
			
			foreach($dados as $numline => $line) {
				unset($n);
				$columns = explode(";", $line);
// 	ver($columns, d);
				if($columns[1] != ''){
					$where = '';
					$valor = '';
					if( $columns[0] !== '' ){ //trata codigo do item
						$itccodigoitem = $columns[0]; 
						$arr = explode('.', $columns[0]);
						foreach($arr as $arrN){
							
							if( $arrN !== '0' ){
								$l = strlen(trim($arrN));
								if( $l == 1 ){
									$n .= '00'.trim($arrN);
								}elseif( $l == 2 ){
									$n .= '0'.trim($arrN);
								}else{
									$n .= trim($arrN);
								}
							}
						}
						$columns[0] = $n;
					}
					
					$columns[2] = (int)$columns[2];
					if( $columns[2] > 0 ){ //verifica a unidade de medida
						if( $columns[2] > 0 ){
							$where .= ", umdid";
							$valor .= ", '{$columns[2]}'";
						}
					}
					
					if( $columns[3] !== '' ){ //trata a quantidade
						$columns[3] = str_replace('.', '', $columns[3]);
						$columns[3] = trim(str_replace(',', '.', $columns[3]));
						if( $columns[3] && $k == 0 ){
							$where .= ", itcquantidade";
							$valor .= ", {$columns[3]}";
						}
					}
					
					if( $columns[4] !== '' ){ //trata a valor
						$columns[4] = str_replace('.', '', $columns[4]);
						$columns[4] = trim(str_replace(',', '.', $columns[4]));
						if( $columns[4] && $k == 0 ){
							$where .= ", itcvalorunitario";
							$valor .= ", {$columns[4]}";
						}
					}
					
					//salvando
// 					$str = addslashes($columns[1]);
					$str = str_replace(Array('\''), Array('\'\''), $columns[1]);
	
					$sql = "SELECT itcid FROM obras.preitenscomposicao WHERE itccodigoitemcodigo = '{$columns[0]}' AND ptoid = $ptoid";
					
					$itcid = $db->pegaUm( $sql );

					if( !$itcid ){
						
						$codpai = substr($columns[0], 0, -3);
						
						if( $codpai != '' ){
							$sql = "SELECT itcid FROM obras.preitenscomposicao WHERE ptoid = {$ptoid} AND itccodigoitemcodigo = '".$codpai."'";
							$pai = $db->pegaUm( $sql );
						
							if( $pai != '' ){
								$where .= ', itcidpai';
								$valor .= ', '.$pai;
							}
						}
						
						$sql = "INSERT INTO obras.preitenscomposicao(
							            itcdescricao, itcstatus, itcdtinclusao, ptoid
							            {$where}, itctipofundacao, 
							            itccodigoitem, itccodigoitemcodigo)
							    VALUES (removeacento('{$str}'), 'A', now(), {$ptoid}
							    		{$valor}, '{$_POST['itctipofundacao']}',
							    		'{$itccodigoitem}', '{$columns[0]}')
								RETURNING itcid;";
						
						$itcid = $db->pegaUm( $sql );
						$db->commit();
					}
					
					if( $k != 0 && $columns[4] > 0){
						$sql = '';
						foreach( $ufs as $uf ){
							
							$sqlTeste = "SELECT true FROM obras.preitencomposicao_regiao 
										 WHERE itcid = (SELECT itcid FROM obras.preitenscomposicao WHERE itccodigoitem = '$itccodigoitem' AND ptoid = $ptoid) AND estuf = '$uf';";
								
							$possuiItemUF = $db->pegaUm( $sqlTeste );
							if( $possuiItemUF != 't' ){
								
								$sql .= "INSERT INTO obras.preitencomposicao_regiao(itcid, pirvalor, pirqtd, muncod, estuf)
										VALUES ($itcid, {$columns[4]}, {$columns[3]}, NULL, '$uf');";
							}
						}
						if( $sql != '' ){
							
							$db->executar($sql);
						}
					}
					$db->commit();
				}
			}
		}

		if($db->commit()){
			echo "<script>
					alert('Operação Realizada com Sucesso!');
					window.location.href = window.location.href;
				 </script>";
			exit();
		} else {
			$db->rollback();
			echo "<script>
					alert('Falha na Operação');
					window.location.href = window.location.href;
				 </script>";
			exit();
		}
	}
	exit();
}

function adicionarLinhaArquivo(){
	
	global $db;
	
	$_POST['num'] = $_POST['num'] ? $_POST['num'] : 1;
	
	for ($i = 1; $i <= $_POST['num']; $i++) {
?>
	<tr>
		<td style="border-top: 1px solid #dcdcdc; ">
			<input type="file" class="arquivo" name="arquivo_<?=($_POST['numero']+($i-1)) ?>" id="arquivo_<?=($_POST['numero']+($i-1)) ?>">
			<?php
			$sql = "SELECT DISTINCT
						estuf as codigo,
						estuf as descricao
					FROM
						territorios.estado
					ORDER BY
						2";
				
			$db->monta_checkbox('estuf['.($_POST['numero']+($i-1)).'][]', $sql, '', $separador='  ', $comLabel = false);
			?>
		</td>
		<td>
			<img  border="0" style="cursor:pointer" title="Excluir" class="excluir" src="../imagens/excluir_2.gif">
		</td>
	</tr>
<?php 
	}
}


if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
//cte_montaTitulo( 'Carga de dados Financeiros', '&nbsp;' );

?>
<!-- <link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" /> -->

<!-- <script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script> -->
<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script>
$(document).ready(function(){

// 	jQuery("#tipo_regiao").addClass("required");
// 	jQuery("#ptodescricao").addClass("required");
// 	jQuery("#pcoid").addClass("required");
// 	jQuery("#tooid").addClass("required");
// 	jQuery("#pooid").addClass("required");
// 	jQuery("#tpoid").addClass("required");
// 	jQuery("#ptopercentualempenho").addClass("required");
// 	jQuery("#itctipofundacao").addClass("required");
// 	jQuery("#ptoprojetofnde").addClass("required");
// 	jQuery("#ptoesfera").addClass("required");
	
    jQuery("#tipo_regiao").attr("required","required");
    jQuery("#ptodescricao").attr("required","required");
    jQuery("#pcoid").attr("required","required");
    jQuery("#pooid").attr("required","required");
    jQuery("#tpoid").attr("required","required");
    jQuery("#ptopercentualempenho").attr("required","required");
    jQuery("#itctipofundacao").attr("required","required");
    jQuery("#ptoprojetofnde").attr("required","required");
    jQuery("#ptoesfera").attr("required","required");
    jQuery("#ptopreencher").attr("required","required");

    $('#formulario').on('change', '.arquivo', function(){
        if( $('#ptoplanilhaporregiao').val() == 'E' ){
	        var arquivo = $(this).val().split(' - ');
	        var id 		= $(this).attr('id').split('_');
			$('#estuf'+id[1]+'_'+arquivo[0]).attr('checked',true);
        }
	});
	
	$('#ptoplanilhaporregiao').change(function(){
		$('.N').hide();
		$('.E').hide();
		$('.'+$(this).val()).show();
		if( $(this).val() == 'N' ){
			$('#arquivocsv').attr('required','required');
			$('#arquivo_1').removeAttr('required');
		}else{
			$('#arquivocsv').removeAttr('required');
			$('#arquivo_1').attr('required','required');
		}
	});

	$('#formulario').on('click', '[name*="estuf["]', function(){
		$('[name*="estuf["][name!="'+$(this).attr('name')+'"][value="'+$(this).val()+'"]').attr('checked',false);
	});

	$('.mais').click(function(){

		var num = $('#novo').val();
		
		var numero = parseFloat($('#numero').val())+1;
		$('#numero').val(numero);
		
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: '&req=adicionarLinhaArquivo&numero='+numero+'&num='+num,
	   		async: false,
	   		success: function(resp){
	   			$('.ultima').before(resp);
	   		}
	 	});
	});

	$('#formulario').on('click', '.excluir', function(){
		$(this).parent().parent().remove();
	})

	$('#enviar').click(function(){

		if( $('[name*="estuf"]:checked').size() != 27 && $('#tipo_regiao').val() == 'E' ){
			alert('Favor anexar arquivos para todos os 27 UFs');
			return false;
		}

		$('#req').val('enviarArquivo');
	});
});

function limparPorEstado(estuf) {
	if(estuf) {
		var conf = confirm("Deseja realmente excluir os dados do estado: "+estuf);
		if(conf) {
			window.location='cte.php?modulo=principal/cargarelatoriocsv01&acao=A&requisicao=limparEstado&estuf='+estuf;
		}
	}
}

function limparPorCarga(datetime) {
	if(datetime) {
		var conf = confirm("Deseja realmente excluir os dados desta carga");
		if(conf) {
			window.location='cte.php?modulo=principal/cargarelatoriocsv01&acao=A&requisicao=limparCarga&datetime='+datetime;
		}
	}
}


</script>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" >
	<input type="hidden" id="req" name="req" value="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">PAR - Carga de Planilha Orçamentária</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo Região:</td>
			<td>
				<?php
					$arr = array( 
									array( "codigo" => "N", "descricao" => "Nacional" ),
									array( "codigo" => "E", "descricao" => "Estadual" ) 
								);
					$db->monta_combo( "ptoplanilhaporregiao", $arr, 'S', 'Selecione..', '', '', '', '', '', 'ptoplanilhaporregiao' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="color:red">Formato do arquivo:</td>
			<td>
				Deve ser uma planilha .csv e possuir 5 colunas com as respectivas informações:<br>
				1 - Código ordinal do item separado por pontos (Ex.: 1.1.21)<br>
				2 - Descrição do item	(NÃO PODE CONTER PONTO E VIRGULA ';' )<br>
				3 - O código da unidade de medida (Ex.: Se for Unidade então deve estar assim: 2)<br>
				4 - Quantidade em formato brasileiro (Ex.: 1.112,23)<br>
				5 - Valor unitário do item contendo somente numeros no formato brasileiro (Ex.: 1.123,23)<br>
				Dicas:<br>
				- Ao incluir arquivos para a espera estadual, se colocar no inicio do arquivo o UF em letras maiusculas separado do nome por ' - ' (Ex.: AC - PORTO VELHO.csv) o sistema marca o UF automaticamente.<br>
			</td>
		</tr>
		<tr class="E" <?=$_REQUEST['tipo_regiao']!='estadual' ? 'style=display:none' : '' ?>>
			<td class="SubTituloDireita" style="vertical-align: top">
				Arquivos: <br>
				<input 	type="text" class=" normal" title="" id="novo" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" 
						onmouseover="MouseOver(this);" value="1" onkeyup="this.value=mascaraglobal('##',this.value);" maxlength="2" size="2" name="novo" style="width: 35px; height: 23px;" >
				<a style="size:10px;v-align:middle;cursor:pointer" class=mais ><img border=0 width=13px src="../imagens/mais.png" class=mais style=cursor:pointer /> Novo Arquivo</a>
			</td>
			<td>
				<table class="tabela" bgcolor="#f5f5f5" width="100%" cellPadding="3"	align="center">
					<tr>
						<td>
							<input type="file" class="arquivo" name="arquivo_1" id="arquivo_1" <?=$_REQUEST['tipo_regiao']!='estadual' ? '' : 'required="required"' ?>>
							<?php 
							$sql = "SELECT DISTINCT
										estuf as codigo,
										estuf as descricao
									FROM
										territorios.estado
									ORDER BY
										2";
							
							$db->monta_checkbox('estuf[1][]', $sql, '', $separador='  ', $comLabel = false);
							?>
						</td>
						<td></td>
					</tr>
					<tr class="ultima"><td><input type="hidden" id="numero" value="1" /></td><td></td></tr>
				</table>
			</td>
		</tr>
		<tr class="N" <?=$_REQUEST['tipo_regiao']!='nacional' && $_REQUEST['tipo_regiao']!='' ? 'style=display:none' : '' ?>>
			<td class="SubTituloDireita">Arquivo:</td>
			<td><input type="file" name="arquivo_0" id="arquivocsv" <?=$_REQUEST['tipo_regiao']!='nacional' && $_REQUEST['tipo_regiao']!='' ? '' : 'required="required"' ?>></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo da Obra:</td>
			<td><?php echo campo_texto( 'ptodescricao', 'N', 'S', '', 50, 200, '', '','','','','id="ptodescricao"'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Preencher?</td>
			<td>
				<?php
					$sql = "SELECT
								'TRUE' as codigo,
								'SIM' as descricao
							UNION ALL
							SELECT
								'FALSE' as codigo,
								'NÃO' as descricao;";
					$db->monta_combo( "ptopreencher", $sql, 'S', 'Selecione..', '', '', '', '', '', 'ptopreencher' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Classificação da Obra(Antigo):</td>
			<td>
				<?php
					$arr = array( 
									array( "codigo" => "P", "descricao" => "Próinfancia" ),
									array( "codigo" => "Q", "descricao" => "Quadra" ), 
									array( "codigo" => "C", "descricao" => "Cobertura" ), 
									array( "codigo" => "E", "descricao" => "Escola" )
								);
					$db->monta_combo( "ptoclassificacaoobra", $arr, 'S', 'Selecione..', '', '', '', '', '', 'ptoclassificacaoobra' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Classificação da Obra:</td>
			<td>
				<?php
					$sql = "SELECT
								pcoid as codigo,
								pcodescricao as descricao
							FROM
								obras.preclassificacaoobra
							WHERE
								pcostatus = 'A'";
					$db->monta_combo( "pcoid", $sql, 'S', 'Selecione..', '', '', '', '', '', 'pcoid' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo da Origem da Obra:</td>
			<td>
				<?php
					$sql = "SELECT
								tooid as codigo,
								toodescricao as descricao
							FROM
								obras.tipoorigemobra 
							WHERE
								toostatus = 'A'";
					$db->monta_combo( "tooid", $sql, 'S', 'Selecione..', '', '', '', '', '', 'tooid' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Origem da Obra:</td>
			<td>
				<?php
					$sql = "SELECT
								pooid as codigo,
								poodescricao as descricao
							FROM
								obras.preorigemobra 
							WHERE
								poostatus = 'A'";
					$db->monta_combo( "pooid", $sql, 'S', 'Selecione..', '', '', '', '', '', 'pooid' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipologia da Obra:</td>
			<td>
				<?php
					$sql = "SELECT 
								tpoid as codigo, 
								tpodsc as descricao 
							FROM 
								obras.tipologiaobra 
							WHERE 
								tpostatus = 'A'";
					$db->monta_combo( "tpoid", $sql, 'S', 'Selecione..', '', '', '', '', '', 'tpoid' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Percentual:</td>
			<td><?php echo campo_texto( 'ptopercentualempenho', 'N', 'S', '', 5, 100, '###', '','','','','id="ptopercentualempenho"'); ?> %</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Fundação:</td>
			<td>
				<?php
					$arr = array( 
									array( "codigo" => "E", "descricao" => "Estaca" ),
									array( "codigo" => "S", "descricao" => "Sapata" ) 
								);
					$db->monta_combo( "itctipofundacao", $arr, 'S', 'Selecione..', '', '', '', '', '', 'itctipofundacao' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Projeto FNDE?</td>
			<td>
				<?php
					$arrFNDE = array( 
									array( "codigo" => 't', "descricao" => "Sim" ),
									array( "codigo" => 'f', "descricao" => "Não" ) 
								);
					$db->monta_combo( "ptoprojetofnde", $arrFNDE, 'S', 'Selecione..', '', '', '', '', '', 'ptoprojetofnde' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Esfera:</td>
			<td>
				<?php
					$arrEsfera = array( 
									array( "codigo" => "E", "descricao" => "Estadual" ),
									array( "codigo" => "M", "descricao" => "Municipal" ),
									array( "codigo" => "T", "descricao" => "Todos" ) 
								);
					$db->monta_combo( "ptoesfera", $arrEsfera, 'S', 'Selecione..', '', '', '', '', '', 'ptoesfera' );
				?>
			</td>
		</tr>
		<tr style="background-color: #DCDCDC">
			<td  align="center" colspan="2">
				<input type="submit" value="Enviar" id="enviar">
			</td>
		</tr>
	</table>
</form>