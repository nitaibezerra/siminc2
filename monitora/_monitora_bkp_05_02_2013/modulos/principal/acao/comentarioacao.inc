<?php
function redirecionar( $modulo, $acao, $parametros = array() ) {

	$parametros = http_build_query( (array) $parametros, '', '&' );
	header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
	exit();
}

/*
 * Correção Alexandre Dourado 16/11/2009
 *  - Validando se existe alguma ação(acaid) selecionada 
 */
if(!$_SESSION['acaid']) {
	echo "<script>
			alert('Ação não selecionada corretamente!');
			window.location='monitora.php?modulo=inicio&acao=C';
		  </script>";	
	exit;
}

//$parametros = array(
//	'aba' => $_REQUEST['aba'], # mantém a aba ativada
//	'atiid' => $_REQUEST['atiid'] 
//);

switch( $_REQUEST['evento'] ){

	case 'cadastrar_nota':
		$sql = sprintf(
			"INSERT INTO monitora.comentarioacao ( cmadescricao, acaid, usucpf ) VALUES ( '%s', %d, '%s' )",
			$_REQUEST['cmadescricao'],
			$_SESSION['acaid'],
			$_SESSION['usucpf']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/acao/comentarioacao&acao=A\';
			 </script>');
		break;
		
	case 'editar_nota':
		$sql = sprintf(
			"UPDATE monitora.comentarioacao SET					
				cmadescricao = '%s',
				cmadata = '%s'
			WHERE cmaid = %d",
			$_POST['hiddenDesc'][$_POST['id']],
			formata_data_sql( date( 'd/m/Y' ) ),
			$_REQUEST['id']
		);
		
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/acao/comentarioacao&acao=A\';
			 </script>');
		break;
		
	case 'excluir_nota':
		$sql = sprintf(
			"UPDATE monitora.comentarioacao SET cmastatus = 'I' where cmaid = %d",
			$_REQUEST['cmaid']
		);
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/acao/comentarioacao&acao=A\';
			 </script>');
		break;

	default:
		break;

}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$arAba = array(0 => array("id" => 1, "descricao" => "Todos os Programas", "link" => "/monitora/monitora.php?modulo=inicio2&acao=C"),
			  1 => array("id" => 2, "descricao" => "Ação",    		 	 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=A&aba=acao&codigo=".$_REQUEST['codigo']),
			  2 => array("id" => 3, "descricao" => "Iniciativa", 		 "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=A&aba=iniciativa&codigo=".$_REQUEST['codigo']),
			  3 => array("id" => 4, "descricao" => "Monitorar Ação", 	 "link" => "/monitora/monitora.php?modulo=principal/acao/monitoraacao&acao=A"),
			  4 => array("id" => 5, "descricao" => "Comentários", 	 "link" => "/monitora/monitora.php?modulo=principal/acao/comentarioacao&acao=A")
		  	  );

echo montarAbasArray($arAba, "/monitora/monitora.php?modulo=principal/acao/comentarioacao&acao=A");		  	  
//$db->cria_aba($abacod_tela,$url,'');
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript">
	
	function cadastrar_nota(){
		if ( validar_formulario_nota() ) {
			document.nota.submit();
		}
	}
	
	function validar_formulario_nota(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		document.nota.cmadescricao.value = trim( document.nota.cmadescricao.value );
		if ( document.nota.cmadescricao.value == '' ) {
			mensagem += '\nConteúdo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluirNota( nota ){
		if ( confirm( 'Deseja excluir o comentário?' ) ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&evento=excluir_nota&cmaid='+ nota;
		}
	}

//	function filtrar_restricoes( filtro ){
//		if ( filtro ) {
//			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>';
//		} else {
//			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&filtro=1';
//		}
//	}

	function enviar_email( cpf ){
		var nome_janela = 'janela_enviar_emai_' + cpf;
		window.open(
			'/geral/envia_email.php?cpf=' + cpf,
			nome_janela,
			'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}
	
	function trim(str)
	{
		return str.replace(/^\s+|\s+$/g,"");
	}
	
	function alteraCampos(id)
	{ 
		top.altera = 'true'; 
 		var antigoValueDesc = trim(document.getElementById('celDescricao_'+id).innerHTML);
 		//var antigoValueData = document.getElementById('celData_'+id).innerHTML;
 		
 		//var inputDesc = "<input type ='text' onkeyup='capturaValorDesc("+id+",this.value);' name='inputDescricao_"+id+" id='inputDescricao_"+id+" value="+antigoValueDesc+">";
 		//var inputData = '<input type ="text"  onkeyup="capturaValorData('+id+',this.value);" name="inputData_'+id+'" id="inputData_'+id+'" value="'+antigoValueData+'"  onkeyup=this.value=mascaraglobal("##/##/####",this.value); >';
 
 		var inputDesc = '<div class="notprint">'+
							'<textarea  id="inputDescricao_'+id+'" name="inputDescricao_'+id+'" cols="10" rows="2"  onkeyup="capturaValorDesc('+id+',this.value);" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this );" style="width:70ex;" onkeydown="textCounter( this.form.inputDescricao_'+id+', this.form.no_inputDescricao_'+id+', 250 );"  onkeyup="textCounter( this.form.inputDescricao_'+id+', this.form.no_inputDescricao_'+id+', 250);" >'+antigoValueDesc+'</textarea>'+ 
								'<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." />'+
								'<br>'+
							'<input readonly style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_inputDescricao_'+id+'" size="6" maxlength="6" value="250" class="CampoEstilo">'+
								'<font color="red" size="1" face="Verdana"> máximo de caracteres</font>'+
						'</div>'+
						'<div id="print_inputDescricao_'+id+'" class="notscreen" style="text-align: left;"></div>'+             
	    	 			'</div>';
	    	 			 
		document.getElementById('celDescricao_'+id).innerHTML = inputDesc;
 		//document.getElementById('celData_'+id).innerHTML = inputData;
 		
 		document.getElementById('altera['+id+']').style.visibility= "hidden";
		document.getElementById('altera['+id+']').style.position= "absolute";
		
		document.getElementById('salva['+id+']').style.visibility= "visible";
		document.getElementById('salva['+id+']').style.position= "relative";
		 
		
	}
	
	function capturaValorDesc(id,value)
	{ 
		document.getElementById('hiddenDesc['+id+']').value = value;
		//document.getElementById('hiddenData['+id+']').value = <?=date("Y-m-d"); ?>;
	}
	function capturaValorData(id,value)
	{
		document.getElementById('hiddenData['+id+']').value = value;
	}
</script>

<script language="javascript" type="text/javascript">
	
	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}
	
</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<!-- NOVO CONTROLE -->
				<form method="post" name="nota">
					<input type="hidden" name="evento" value="cadastrar_nota"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
							<td><?= campo_textarea( 'cmadescricao', 'S', 'S', '', 70, 6, '' ); ?></td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_nota();"/></td>
						</tr>
					</table>
				</form>
			
			<!-- LISTA DE RESTRIÇÕES -->
			<?php
				$sql = sprintf(
								"SELECT
									ca.cmaid, ca.cmadescricao, ca.cmadata,
									usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum
								FROM monitora.comentarioacao ca
								LEFT JOIN seguranca.usuario usu ON usu.usucpf = ca.usucpf
								WHERE
									ca.acaid = %d AND 
									ca.cmastatus = 'A'
								ORDER BY ca.cmadata DESC, ca.cmaid DESC",
								$_SESSION['acaid']
							);
				$lista = $db->carregar( $sql );
				
				//dbg( $lista );
			?>
			<?php if( is_array( $lista ) ): ?>
				<?php foreach( $lista as $item ): ?>
				<form method="post" id="formulario_<?= $item['cmaid'] ?>">
					<input type="hidden" name="evento" value="editar_nota"/>
					<input type="hidden" name="cmaid" value="<?= $item['cmaid'] ?>"/>
					<table id="<?= $item['cmaid'] ?>" class='tabela' bgcolor="#f5f5f5" style="width:100%; margin-top: 15px;<?= $item['obssolucao'] == 't' ? 'color:#454545;' : '' ?>" cellpadding="3">
						<tbody>
							<tr style="background-color: #cccccc">
								<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
								<td align='left' style="vertical-align:top;"><b>Comentário</b></td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
								<input type="hidden"  id="hiddenDesc[<?= $item['cmaid'] ?>]" name="hiddenDesc[<?= $item['cmaid'] ?>]" value="<?= $item['cmadescricao'] ?>"/>
								<input type="hidden" name="id" value="<?= $item['cmaid'] ?>"/>
								<td id="celDescricao_<?= $item['cmaid'] ?>" name="celDescricao_<?= $item['cmaid'] ?>" >
								 	<?= $item['cmadescricao'] ?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
								<td><?= formata_data( $item['cmadata'] ); ?></td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
								<td>
									<div>
										<img onclick="enviar_email( '<?= $item['usucpf'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
										<?= $item['usunome'] ?>
									</div>
									<div style="color:#959595;"><?= $item['usunome'] ?> - Tel: (<?= $item['usufoneddd'] ?>) <?= $item['usufonenum'] ?></div>
								</td>
							</tr>
								<tr style="background-color: #cccccc">
									<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
									<td>
									<?php
									//dbg( $item);
									if( $item['usucpf'] == $_SESSION['usucpf'])
									{
									?>
										<input type="button" name="altera[<?= $item['cmaid'] ?>]" id="altera[<?= $item['cmaid'] ?>]" value="Alterar" onclick="alteraCampos(<?= $item['cmaid'] ?>);"/>
										<input type="submit" name="salva[<?= $item['cmaid'] ?>]" id="salva[<?= $item['cmaid'] ?>]" value="Salvar" style="visibility: hidden; position:absolute; display:inline;" />
									<?php
									}
									?>
										<input type="button" name="botao" value="Excluir" onclick="excluirNota( <?= $item['cmaid'] ?> );" />
									</td>
								</tr>
						</tbody>
					</table>
				</form>
				<?php endforeach; ?>
			<?php else: ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<tbody>
						<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
							A ação não possui comentários.
						</td>
					</tbody>
				</table>
			<?php endif; ?>
		</td>
	</tr>
</table>