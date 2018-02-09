<!--  
// É necessário que a tela que chamou esta, informe os parametros abaixo:
// 'nomecampo': é o nome do campo da tabela que será utilizado para recupera o texto que será exibido na tela
// 'nometabela': é o nome da tabela que será recuperado o texto
// 'nomeid': é o nome da coluna da tabela que será a chave da busca.
// 'id': é o identificador do registro que será recuperado.

// Esta página é chamada pela /principal/documentosAnalisePTA.inc para exibir os documentos extras da emenda.
-->

<form id="formulario" name="formulario" method="post" >

<table width="100%" class="tabela" align="center">
    <tr>
        <td>
			<?php
				// Recupera o texto do documento
				
				$sql = "SELECT ".$_REQUEST["nomecampo"]."
						FROM ".$_REQUEST["nometabela"]."
						WHERE ".$_REQUEST["nomeid"]." = ".$_REQUEST["id"];
				
				$texto = $db->pegaUm($sql);

			?> 
			<div id="conteudo" style="font-weight:normal;vertical-align:top; FONT: 8pt Arial,verdana">
				<?=html_entity_decode($texto)?>
			</div>
        </td>
    </tr>
</table>
    
</form>
</body>
