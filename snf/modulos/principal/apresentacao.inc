<?php 

include  APPRAIZ."includes/cabecalho.inc";

?>
<style>
<!--
.apresentacaoSinafor{
	width: 90%;
	border: 1px solid black;
	text-align: left;
	margin: 15px;
	padding: 15px;
	font-size: 14px;
}
-->
</style>

<script>

function acessarLista(){
	document.location.href = 'snf.php?modulo=principal/listaEscolas&acao=A';
}

</script>

<center>
	<div class="apresentacaoSinafor" >
	
		<center><h3>Seja bem-vindo ao Sinafor!<h3></h3></center>
		 
		<p>O Sistema Nacional de Formação é o módulo do <?php echo SIGLA_SISTEMA; ?> no qual as Secretarias de Educação realizam a priorização e 
		validação dos Planos de Formação elaborado pelas escolas no módulo PDE Interativo.</p> 
		
		<p>Esta etapa do novo fluxo de demanda e oferta de formação serve para que a direção da Rede de Ensino defina escolas 
		prioritárias, restrinja o número de professores que estarão em formação simultaneamente e concretize seu planejamento 
		do ano, além de já ter dados para iniciar o planejamento dos anos seguintes.</p> 
		
		<p>Os critérios de priorização são definidos por cada Secretaria. Para defini-los, recomenda-se que  a equipe de análise 
		seja formada pelos técnicos responsáveis pelo Plano de Ações Articuladas (PAR), pela formação docente na Secretaria e 
		pelo Comitê do PDE Escola. Este grupo de se reunir e definir os parâmetros de validação.</p> 
		
		<p>A definição de critérios prévios, que serão utilizados para validar as indicações, facilita a análise e, principalmente, 
		torna o processo mais transparente junto às escolas. Ao final desta etapa, o sistema devolverá as informações sobre 
		aprovação para o PDE Interativo e cada docente poderá saber se foi ou não aprovado. Entre os critérios que podem ser 
		considerados, destacam-se: Resultado das escolas no IDEB, localização das escolas,  adequação dos cursos solicitados 
		às necessidades atuais das escolas, investimento anterior nos docentes e gestores ,  experiência individual, tipo de 
		vínculo Profissional , tempo de carreira, comprometimento, apoio institucional e percentual de pessoas indicadas em 
		relação ao total da rede.</p>
		
	</div>
	<input type="button" value="Acessar" onclick="acessarLista()" /> 
</center>