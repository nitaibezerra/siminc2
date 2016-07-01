<?php

/*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Programador: Fernando Bagno (fernandobagno@gmail.com)
*/

if ( $_SESSION["ies"]["iesid"] ){
	$iesid = $_SESSION["ies"]["iesid"];
	$dadosInstituicao = $ies->buscaDadosInstituicao( $_SESSION["ies"]["iesid"] );
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

if ( !iesPegaProjeto($iesid) ){
	
	$menu = array( 0 => array( 'descricao' => 'Página Inicial', 'link' => 'ies.php?modulo=inicio&acao=C' ),
	 			   1 => array( 'descricao' => 'Dados da Instituição', 'link' => 'ies.php?modulo=principal/dadosInstituicao&acao=A' )
	 			  );
	
	echo montarAbasArray($menu, $url = 'ies.php?modulo=principal/dadosInstituicao&acao=A');
	
}else{
	
	$menu = array( 0 => array( 'descricao' => 'Página Inicial',		  'link' => 'ies.php?modulo=inicio&acao=C' ),
	 			   1 => array( 'descricao' => 'Dados da Instituição', 'link' => 'ies.php?modulo=principal/dadosInstituicao&acao=A' ),
	 			   2 => array( 'descricao' => 'Dados do Responsável', 'link' => 'ies.php?modulo=principal/dadosResponsavel&acao=A' ),
	 			   3 => array( 'descricao' => 'Projeto',			  'link' => 'ies.php?modulo=principal/projeto&acao=A' )
	 			  );
	
	echo montarAbasArray($menu, $url = 'ies.php?modulo=principal/dadosInstituicao&acao=A');
	
}
monta_titulo( "Dados da Instituição", "" );

?>
<script language="JavaScript" src="./geral/js/ies.js"></script>

<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td class="subtitulocentro" colspan="4">Mantenedora</td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20%"> (CNPJ) Mantenedora (Código) </td>
		<td colspan="3"><?php echo $dadosInstituicao["mantenedora"]; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Natureza Jurídica</td>
		<td colspan="3"><?php echo $dadosInstituicao["naturezamantenedora"]; ?></td>
	</tr>
	<tr>
		<td class="subtitulocentro" colspan="4">IES</td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20%">Nome da IES (Código)</td>
		<td colspan="3">
			<b><?php echo $dadosInstituicao["nome"]; ?></b>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Endereço</td>
		<td width="30%"><?php echo $dadosInstituicao["endereco"]; ?></td>
		<td class="subtitulodireita" width="20%">Nº</td>
		<td width="30%"><?php echo $dadosInstituicao["numero"]; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Complemento</td>
		<td><?php echo $dadosInstituicao["complemento"]; ?></td>
		<td class="subtitulodireita">CEP</td>
		<td><?php echo formata_cep($dadosInstituicao["cep"]); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Bairro</td>
		<td><?php echo $dadosInstituicao["bairro"]; ?></td>
		<td class="subtitulodireita">Município / UF</td>
		<td><?php echo $dadosInstituicao["municipio"]; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Telefone</td>
		<td><?php echo $dadosInstituicao["tel"]; ?></td>
		<td class="subtitulodireita">Fax</td>
		<td><?php echo $dadosInstituicao["fax"]; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Organização Acadêmica</td>
		<td><?php echo $dadosInstituicao["organizacao"]; ?></td>
		<td class="subtitulodireita">Sítio</td>
		<td><?php echo $dadosInstituicao["sitio"]; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">E-mail</td>
		<td><?php echo $dadosInstituicao["email"]; ?></td>
	</tr>
	<tr>
		<td align="center" colspan="4">
			<p style="font-weight: bold;">
				<br/>
				Os dados acima correspodem às informações constantes do cadastro de instituições e cursos superiores do MEC. <br/>
				Caso haja alguma divergência, solicitamos proceder à atualização.
				<br/><br/>
			</p>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="4">
			<input type="button" value="Continuar" onclick="iesConfirmarDados(<?=$_SESSION["ies"]["iesid"]?>);" style="cursor: pointer;">
		</td>
	</tr>
</table>