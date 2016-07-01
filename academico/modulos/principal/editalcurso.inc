<?php

// objetos do módulo
$curso = new cursos();
$autoriazacaoconcursos = new autoriazacaoconcursos();

// variáveis utilizadas na tela
$curid = $_SESSION["academico"]["curid"];
$edpid = $_REQUEST["edpid"] ? $_REQUEST["edpid"] : '';
$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];  
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid']; 
$_SESSION['academico']['entid'] = $autoriazacaoconcursos->buscaentidade($entidcampus);
$requisicao = $edpid ? 'atualiza' : 'insere';

// trata as requisições da tela
if ( $_REQUEST["requisicao"] == 'insere' ){
	$curso->cadastraEdital( $_REQUEST );
}elseif( $_REQUEST["requisicao"] == 'atualiza' ){
	$curso->atualizaEdital( $_REQUEST, $_FILES );
}

if ( $_REQUEST["excluidocumento"] ){
	$curso->excluiDocumento( $_REQUEST );
}

if ( $_REQUEST["download"] ){
	$curso->downloadArquivo($_REQUEST["arqid"]);
}

if ( !empty($edpid) ){
	$edital = $curso->buscaEdital($edpid);
}

monta_titulo( "Cadastro de Edital do Curso", "");

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

// cria o cabeçalho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus, '', $curid);

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/calendario.js"></script>
<script>
	function curso_edital(){
	
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var validacao = true;
	
		var numero = document.formulario.edpnumero.value;
		if (numero == ''){
			mensagem += 'Nº do Edital \n';
			validacao = false;
		}
		
		var ano = document.formulario.edpano.value;
		if (ano == ''){
			mensagem += 'Ano \n';
			validacao = false;
		}
	
		if (!validacao){
			alert(mensagem);
		}else{
			document.getElementById('formulario').submit();
		}
	
	}
	
	function curso_excluiarquivoedital( anxid, arqid ){
		if(confirm("Deseja realmente excluir este documento?")){
			window.location = 'academico.php?modulo=principal/editalcurso&acao=A&excluidocumento=1&anxid=' + anxid + '&arqid='+arqid;
		}
	}
	
	function curso_downloadarquivoedital( arqid ){
		window.location = 'academico.php?modulo=principal/editalcurso&acao=A&download=1&arqid='+arqid;
	}
	
</script>
<form action="" enctype="multipart/form-data" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" value="<?=$requisicao?>"/>
	<input type="hidden" name="curid" value="<?=$curid?>"/>
	<input type="hidden" name="edpid" value="<?=$edpid?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro">Dados do Edital</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="20%">Nº do Edital</td>
			<td>
				<?php $edpnumero = $edital["edpnumero"]; ?>
				<?=campo_texto( 'edpnumero', 'S', $habil, '', 23, 20, '', '','','','','id="edpnumero"'); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data</td>
			<td>
				<?php $edpdtcriacao = $edital["edpdtcriacao"]; ?>
				<?= campo_data( 'edpdtcriacao', 'N', $habil, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº de Vagas Vestibular</td>
			<td>
				<?php $edpnumvagas = $edital["edpnumvagas"]; ?>
				<?=campo_texto( 'edpnumvagas', 'N', $habil, '', 6, 4, '', '','','','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ano</td>
			<td>
				<?php $edpano = $edital["edpano"]; ?>
				<?=campo_texto( 'edpano', 'S', $habil, '', 6, 4, '', '','','','','id="edpano"'); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº do DOU</td>
			<td>
				<?php $edpnumdiario = $edital["edpnumdiario"]; ?>
				<?=campo_texto( 'edpnumdiario', 'N', $habil, '', 23, 20, '', '','','','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data do DOU</td>
			<td>
				<?php $edpdtpubldiario = $edital["edpdtpubldiario"]; ?>
				<?= campo_data( 'edpdtpubldiario', 'N', $habil, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº da Seção</td>
			<td>
				<?php $edpsecaodiario = $edital["edpsecaodiario"]; ?>
				<?=campo_texto( 'edpsecaodiario', 'N', $habil, '', 12, 10, '', '','','','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº da Página</td>
			<td>
				<?php $edpdiariopagina = $edital["edpdiariopagina"]; ?>
				<?=campo_texto( 'edpdiariopagina', 'N', $habil, '', 12, 10, '', '','','','',''); ?>
			</td>
		</tr>
		<!-- lista os arquivos do edital (caso em atualização) -->		
		<?php if ($edpid){ ?>
		<tr>
			<td colspan="2" class="subtitulocentro">Arquivos do Edital</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Arquivo</td>
			<td><input type="file" name="arquivo"/></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Descrição</td>
			<td><?= campo_textarea( 'arqdescricao', 'N', $habil, '', 60, 2, 250 ); ?></td>
		</tr>
		<tr><td><b>Arquivos</b></td></tr>
		<tr>
			<td colspan="2">
				<?php
					$sql = "SELECT 
								'<center><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"curso_excluiarquivoedital(' || aa.anxid || ', ' || aa.arqid || ');\"></center>' as acao,
								to_char(anxdtinclusao, 'DD/MM/YYYY'),
								'<a style=\"cursor: pointer; color: blue;\" onclick=\"curso_downloadarquivoedital(' || aa.arqid || ');\" />' || arqnome || '.'|| arqextensao ||'</a>',
								arqtamanho,
								usunome
							FROM 
								academico.anexocurso aa
							INNER JOIN
								public.arquivo pa ON aa.arqid = pa.arqid
							INNER JOIN
								seguranca.usuario su ON pa.usucpf = su.usucpf
							WHERE
								anxstatus = 'A' AND edpid = {$edpid}";
					
					$cabecalho = array( "Ação", "Data de Inclusão", "Nome do Arquivo", "Tamanho (kbs)", "Responsável");
					$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
					
				?>
			</td>
		</tr>
		<?php } ?>
		<tr bgcolor="#cccccc">
			<td colspan="2">
				<input type="button" value="Salvar" onclick="curso_edital();" style="cursor: pointer;"/>
				<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>