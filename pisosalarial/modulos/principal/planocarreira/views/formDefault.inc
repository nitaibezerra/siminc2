<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">

	/**
	 * Submete o formulário
	 * @name enviaArquivos
	 * @return void
	 */
	function enviaArquivos( arquivo, tipo ){
		var formulario = $('formulario');

	   extensoes_permitidas = new Array(".pdf", ".doc", ".odt", ".docx");
	   meuerro = "";
	   if (!arquivo) {
	      //Se não tenho arquivo, é porque não se selecionou um arquivo no formulário.
	        meuerro = "Por favor, selecionar o arquivo que deseja anexar para realizar essa ação.";
	   }else{
	      //recupero a extensão deste nome de arquivo
	      extensao = (arquivo.substring(arquivo.lastIndexOf("."))).toLowerCase();
	      //comprovo se a extensão está entre as permitidas
	      permitida = false;
	      for (var i = 0; i < extensoes_permitidas.length; i++) {
	         if (extensoes_permitidas[i] == extensao) {
	         permitida = true;
	         break;
	         }
	      }
	      if (!permitida) {
	         meuerro = "Somente arquivos com extensão .PDF, .DOC, .ODT ou .DOCX  poderão ser anexados.";
	        }else{
	        	$('tipoanexo').setValue( tipo );
				$('requisicao').setValue('salvar');
	         	formulario.submit();
	         	return 1;
	        }
	   }
	   //se estou aqui é porque não se pode submeter
	   alert (meuerro);
	   return 0;
	}

	/**
	 * Excluir o anexo selecionado
	 * @name excluirAnexo
	 * @return void
	 */
	function excluirAnexo( gerdata, gerid, arqid ){
		$('gerdata').setValue( gerdata );
//		if( !validaDataMaior( $('dataatual'), $('gerdata')  ) ){
//			alert("De acordo com a data do anexo do arquivo, esse registro não pode ser excluído.");
//			return false;
//		}
		$('requisicao').setValue('excluir');
		$('arqid').setValue(arqid);
		$('gerid').setValue(gerid);
		$('formulario').submit();
	}

	/**
	 * Visualizar o anexo selecionado
	 * @name visualizaAnexo
	 * @return void
	 */
	function visualizaAnexo( arqid ){
		$('requisicao').setValue('download');
		$('arqid').setValue(arqid);
		$('formulario').submit();
	}
</script>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
	<input type="hidden" name="tipoanexo" id="tipoanexo" value="">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="gerid" id="gerid" value="">
	<input type="hidden" name="arqid" id="arqid" value="">
	<input type="hidden" name="dataatual" id="dataatual" value="<?=date('d/m/Y'); ?>">
	<input type="hidden" name="gerdata" id="gerdata">
    <?php if(possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL))): ?>
	<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita" style="width: 33%"><b>Faça upload de no mínimo um dos arquivos abaixo.</b></td>
			<td><b>Ato de criação do plano de carreira para o ministério.</b></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" style="width: 33%">Lei municipal (anexar arquivo)</td>
			<td><input type="file" name="arquivolei" id="arquivolei" />
				<input type="button" name="botao" value="Enviar" onclick="enviaArquivos(this.form.arquivolei.value, 'LE');"/></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" style="width: 33%">Decreto (anexar arquivo)</td>
			<td><input type="file" name="arquivodecreto" id="arquivodecreto" />
				<input type="button" name="botao" value="Enviar" onclick="enviaArquivos(this.form.arquivodecreto.value,'DE');"/></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" style="width: 33%">Outros (anexar arquivo)</td>
			<td><input type="file" name="arquivooutro" id="arquivooutro" />
				<input type="button" name="botao" value="Enviar" onclick="enviaArquivos(this.form.arquivooutro.value, 'OU');"/></td>
		</tr>
	</table>
    <?php endif; ?>
</form>