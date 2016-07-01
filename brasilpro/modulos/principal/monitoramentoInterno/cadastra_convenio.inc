<?php

if( $_POST['acao'] == 'gravar' || $_POST['acao'] == 'salvar' ){
	
	$duplicado = $db->pegaUm("SELECT
								count(*)
							  FROM
							  	cte.conveniomonitoramento
							  WHERE
							  	cmodsc = '{$_POST['cmodsc']}' AND
							  	cmoano =  {$_POST['cmoano']}");
	
	if( $duplicado > 0 && $_POST['acao'] == 'gravar' ){
		
		echo "Este convenio já existe.(Código e ano existentes)";
		die();
		
	}else{
		
		if( $_POST['acao'] == 'salvar' ){
			
			$sql = "UPDATE 
						cte.conveniomonitoramento
					SET 
						cmosigla = '{$_POST['cmosigla']}',
						cmodsc 	 = '{$_POST['cmodsc']}',
						cmoano   =  {$_POST['cmoano']},
						estuf    = '{$_POST['estuf']}'
					WHERE 
						cmoid = {$_POST['cmoid']}";
			
			$db->executar($sql);
			$db->commit();
	
			echo "Convênio alterado.";
			
		}else{
			
			$sql = "INSERT INTO 
						cte.conveniomonitoramento(cmosigla,cmodsc, cmoano, estuf)
					VALUES 
						('{$_POST['cmosigla']}','{$_POST['cmodsc']}', {$_POST['cmoano']}, '{$_POST['estuf']}')";
			
			$db->executar($sql);
			$db->commit();
			
			echo "Convênio criado.";
			
		}
		
	}
	die();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$res = array( 0 =>
					array ("descricao" => "Pesquisa de Convênios",
							    "id"        => "1",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_convenio&acao=A"
							   )
			);

if( $db->testa_superuser() ){	
		array_push($res,
					array ("descricao" => "Cadastrar Covênios",
							    "id"        => "2",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_convenio&acao=A"
							   ),
		   			array ("descricao" => "Vincular Convênios nas Subações",
							    "id"		=> "3",
							    "link"		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_convenio&acao=A"
					  		   )
			  );
	}
	
echo montarAbasArray($res, "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_convenio&acao=A");

?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>

<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td width="100%" align="center" gbcolor="#dcdcdc" colspan="3" bgcolor="#dcdcdc" >
			<label class="TituloTela" style="color: rgb(0, 0, 0);">Cadastro de convênios</label>
		</td>
	</tr>
	<tr>
		<td style="" align="center" bgcolor="#e9e9e9" colspan="3">
			&nbsp;
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width='10%'>
			Sigla:
		</td>
		<td width='50%' valign="center">
			<?php 
				$sql = "SELECT DISTINCT
							cmosigla as codigo,
							cmosigla || '' as descricao
						FROM
							cte.conveniomonitoramento
						ORDER BY
							cmosigla";
				
				$db->monta_combo( 'combocmosigla', $sql, 'S', 'Selecione', 'carregaSigla', '' ,'','100','N','combocmosigla', '', $_REQUEST["cmosigla"] );
			?>
			&nbsp; ou &nbsp;
			<?php 
			$_REQUEST['cmosigla'] = $_REQUEST['cmosigla'] ? $_REQUEST['cmosigla'] : ''; 
			?>
			<?= campo_texto( 'cmosigla', 'S', 'S', '', 8, 5, '', '','','','','id="cmosigla"','this.value = somenteTexto(this);','','' ); ?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width='10%'>
			Código / Ano:
		</td>
		<td width='50%' valign="center">
			<?php 
			$_REQUEST['cmodsc'] = $_REQUEST['cmodsc'] ? $_REQUEST['cmodsc'] : ''; 
			$_REQUEST['cmoano'] = $_REQUEST['cmoano'] ? $_REQUEST['cmoano'] : ''; 
			?>
			<?= campo_texto( 'cmodsc', 'S', 'S', '', 26, 26, '####################', '','','','','id="cmodsc"','','','this.value=mascaraglobal(\'####################\',this.value);' ); ?>
			&nbsp; / &nbsp;
			<select id="cmoano" name="cmoano" onchange="" >
				<option value="<?=$_REQUEST['cmoano']?>"><?=$_REQUEST['cmoano'] ?></option>
				<option value=""></option>
				<?php for($x=2008;$x<=date('Y');$x++): ?>
					<option value="<?php echo $x ?>"><?php echo $x ?></option>
				<?php endfor; ?>
			</select>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
		<td></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
			UF:
		</td>
		<td>
			<?php 
			$sql = "SELECT
						estuf AS codigo,
						estuf || ' - ' || estdescricao AS descricao
					FROM
						territorios.estado
					ORDER BY
						estuf";
	
	 		echo $db->monta_combo('estuf',$sql,'S',"-- Selecione para filtrar --",'','','',170,'','estuf');
			?>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
		</td>
		<td>
		<?php 
		if( $_REQUEST['req'] == 'importar' ){
		?>
			<input type="button" id="btnSalvar" name="btnSalvar" value="Salvar Alterações" onclick="salvarConv()"/>
		<?php 
		}else{
		?>
			<input type="button" id="btnCriar" name="btnCriar" value="Gravar Novo" onclick="criarConv()"/>
		<?php 
		}
		?>
			<input type="hidden" id="acao"      name="acao" 	 value="0"></input>
			<input type="hidden" id="cmoid"     name="cmoid" 	 value="0"></input>
			<input type="button" id="btnVoltar" name="btnVoltar" value="Voltar" onclick="voltarInicio()"/>
		</td>
		<td></td>
	</tr>
</table>
</form>
<script>

////completa zeros no campo
//function completaZero( objeto ){
//
//	var tamanho = objeto.value.length;
//	var restante = 4 - tamanho;
//	for(i = 0; i<restante; i++){
//		objeto.value = '0'+objeto.value;
//	}
//}

//Redireciona para a primeira página.
function voltarInicio(){
	window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_convenio&acao=A';
}

//Valida o Formulário
function validaForm() {		

	var cmosigla = document.getElementById('cmosigla');
	var cmodsc   = document.getElementById('cmodsc');
	var cmoano   = document.getElementById('cmoano');
	var estuf    = document.getElementById('estuf');
	
	if ( cmosigla.value == '' ){
		alert('Campo obrigatório.');
		cmosigla.focus();
		return false;
	}else{
		var regExp   = /^[a-zA-Z]{1,5}$/; 
	
		testa = regExp.exec(cmosigla.value);
		
		if(!testa){
			alert('Formato inválido (Apenas letras).');
			cmosigla.value = cmosigla.value.toUpperCase();
			cmosigla.focus();
			return false
		}
	}

	if ( cmodsc.value == '' ){
		alert('Campo obrigatório.');
		cmodsc.focus();
		return false;
	}

	if ( cmoano.value == '' ){
		alert('Campo obrigatório.');
		cmoano.focus();
		return false;
	}

	if ( estuf.value == '' ){
		alert('Campo obrigatório.');
		estuf.focus();
		return false;
	}

	
}	

function criarConv(){

	if ( validaForm() != false ){
	
		$('acao').value = 'gravar';
		
		return new Ajax.Request(window.location.href,{
			method: 'post',
			parameters: $('formulario').serialize('true'),
			onComplete: function(res){
				alert(res.responseText);
				window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_convenio&acao=A';
			}
		});			

	}
}

function salvarConv(){

	if ( validaForm() != false ){

		$('acao').value = 'salvar';
		
		return new Ajax.Request(window.location.href,{
			method: 'post',
			parameters: $('formulario').serialize('true'),
			onComplete: function(res){
				alert(res.responseText);
				window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_convenio&acao=A';
			}
		});

	}
				
}

function somenteTexto( objeto ){

	var texto = objeto.value;
	var tamanho = objeto.value.length-1;
	var regExp = /^[a-zA-Z]{1,5}$/; 

	testa = regExp.exec(texto);

	if(!testa){
		texto = texto.substring(0, tamanho);
	}

	texto = texto.toUpperCase();
	
	return texto;
}

function carregaSigla( sigla ){

	$('cmosigla').value = sigla;
				
}

</script>
<?php 

if( $_REQUEST['req'] == 'importar'){
	
	$sql = "SELECT 
				cmosigla,
				cmodsc,
				cmoano,
				estuf
			FROM 
				cte.conveniomonitoramento
			WHERE 
				cmoid = ".$_REQUEST['cmoid'];
	
	$dados = $db->pegaLinha($sql);
	
	echo "<script> document.getElementsByName( 'combocmosigla' )[0].value = '".$dados['cmosigla']."';</script>";
	echo "<script> document.getElementsByName( 'cmosigla' )[0].value = '".$dados['cmosigla']."';</script>";
	echo "<script> document.getElementsByName( 'cmodsc' )[0].value = '".$dados['cmodsc']."';</script>";
	echo "<script> document.getElementsByName( 'cmoano' )[0].value = '".$dados['cmoano']."';</script>";
	echo "<script> document.getElementsByName( 'estuf' )[0].value  = '".$dados['estuf']."';</script>";
	echo "<script> document.getElementsByName( 'cmoid' )[0].value  = '".$_REQUEST['cmoid']."';</script>";
	
}

?>
