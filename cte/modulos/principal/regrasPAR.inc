<?php
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;');

?>
<style>
 ul{
 	list-style-type:decimal-leading-zero;
 }
 
 li{
 	font-size: 12px;
 	font-family: arial;
 	font-style: normal;
 	
 }
 
 table.tabela2{
 	background-color: white;
 }
</style>
<table align="center" border="0" class="tabela tabela2" cellpadding="3" cellspacing="1">
	<tr>
		<th>Lista de municípios</th>
	</tr>
	<tr>
		<td></td>
	</tr>
	<tr>
		<th>Lista de Estados</th>
	</tr>
	<tr>
		<td></td>
	</tr>
	<tr>
		<th>Tela de Pontuação</th>
	</tr>
	<tr>
		<td>
			<ul>
				<li>Se for estado ele pode ou não seguir o quia. O município e obrigado a sequir o que está no guia do PAR.</li>
				<li>Se a pontuação for 3 ou 4 abre apenas para o Administrador o botão de proposta de subações onde ele pode inserir ações e subaçãoes propostas no quia.</li>
			</ul>
		</td>
	</tr>
	<tr>
		<th>Tela de Subaçãoes</th>
	</tr>
	<tr>
		<td>
			<ul>
				<li>Se a subação / ano estiver conveniada então ela fica travada apenas no ano que foi convêniada e o botão de inserir adtivos abre.</li>
				<li>Se o ano for menor que o ano corrente ela trava - So fica liberada para Administradores e Super Usuário.</li>
				<li>Se a subação / ano estiver com o status do parecer lançado e o município estiver na fase de elaboração a subção fica travado.</li>
				<li>Se a subação for de assistência financeira e não tiver nada preenchido então não há pendência em nenhuma fase.</li>
				<li>Se a subação for de assistência técnica Pelo menos em um ano têm que ter dados lançados se não da erro de pendencia em qualquer fase. </li>
			</ul>
		</td>
	</tr>
	<tr>
		<th>Regras para geração de termo de cooperação e pareceres</th>
	</tr>
	<tr>
		<td>
			<ul>
				<li>Apenas administradores podem gerar o termo e o parecer.</li>
			</ul>
		</td>
	</tr>
</table>
