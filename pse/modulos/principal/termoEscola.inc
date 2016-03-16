<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
VerSessao();
$entid = $_SESSION['pse']['entid'];

$sql ="	SELECT	ent.entid,entd.entcodent, ent.entnome, ende.estuf, mun.mundescricao,ende.endlog,ende.endcom,ende.endbai,
				ent.entnumcpfcnpj,professor.entid as identf, professor.entnome as dir, professor.entnumrg as profrg, professor.entnumcpfcnpj as profcpf,
				professor.entorgaoexpedidor as exprg  
		FROM entidade.entidade ent				
		LEFT JOIN 
			entidade.funcaoentidade feEscola on feEscola.entid = ent.entid
		LEFT JOIN 
			entidade.funentassoc assocprofessores on assocprofessores.entid = ent.entid
		LEFT JOIN 
			entidade.funcaoentidade feProfessor on feProfessor.fueid = assocprofessores.fueid	
		LEFT JOIN 
			entidade.entidade professor on professor.entid = feProfessor.entid
		LEFT JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			entidade.entidade2 ent2 ON ent2.entid = ent.entid
		INNER JOIN
			entidade.entidadedetalhe entd ON entd.entid = ent.entid		
		INNER JOIN
			territorios.municipio mun ON mun.muncod = ende.muncod
		INNER JOIN
			territorios.estado est ON est.estuf = mun.estuf
		WHERE	ent.entstatus='A' AND 
				ent2.funid = 3
				and ent.entid = '$entid'";
				
		$dados	= $db->pegalinha($sql);
		
		$escola			= ($dados['entnome']==''?'______________________________________':$dados['entnome']);
		$diretor		= ($dados['dir']==''?'______________________________________':$dados['dir']);
		$cnpj			= ($dados['entnumcpfcnpj']==''?'______________________':$dados['entnumcpfcnpj']);
		$inep			= ($dados['entcodent']==''?'_______________':$dados['entcodent']);
		$conselho		= ($dados['conselho']==''?'___________________________________':$dados['conselho']);
		$rg				= ($dados['profrg']==''?'_______________':$dados['profrg']);
		$exprg			= ($dados['exprg']==''?'_______________':$dados['exprg']);
		$cpf			= ($dados['profcpf']==''?'__________________':$dados['profcpf']);
		$endconselho	= ($dados['endconselho']==''?'______________________________________________________':$dados['endconselho']);
		
		
		
?>
<STYLE TYPE="text/css">
.quebra {
    page-break-after: always;
}
</STYLE>
<p align="right"><a href="#" onclick="self.print();">Imprimir</a></p>
<body onload="self.print();">
<table align="center" cellspacing="1" cellpadding="4">
	<tr>
		<td align="center"><img src="../imagens/brasao.gif" border="0" width="100" height="100"></td>
	</tr>
	<tr>
		<td align="center">
			<b>Ministério da Educação<br>
			Secretaria de Educação Continuada, Alfabetização e Diversidade<br>
			Diretoria de educação Integral, Direitos Humanos e Cidadania</b>
		</td>
	</tr>
	<tr>
		<td align="center">
			<br><br><br><br>
			<b>TERMO DE COMPROMISSO</b>
			<br><br><br><br>
		</td>
	</tr>
</table>
<br><br><br>
<table align="center" cellspacing="10" cellpadding="5">
<tr>
<td>
<p align="justify">

Entre <br>
<b>Ministério da Educação</b><br>
<b>Secretaria de Educação Continuada, Alfabetização e Diversidade – SECAD/MEC</b>
<br><br>
Localizado na<br> 
Esplanada dos Ministérios<br> 
SECAD/MEC<br>
Bloco L – 6º andar<br>
70070-914 Brasília, Brasil
<br><br>
E
<br><br>
Escola <b><?=$escola;?>/Código INEP nº <?=$inep;?></b>, inscritas no CNPJ sob nº <?=$cnpj;?>,
neste ato representada por seu(sua) Diretor(a)<b><?=$diretor;?></b>, portador(a) 
da carteira de identidade nº <?=$rg;?>, expedida por <?=$exprg;?>, e inscrito(a) no CPF sob o nº <?=$cpf;?>,
Em co-responsabilidade com seu Conselho Escolar <b><?=$conselho;?></b>, 
localizado na <?=$endconselho;?>.
<br><br> 
Em concordância com o estabelecido na Portaria nº......../2009 de adesão ao Programa Saúde na Escola relativamente 
ao apoio institucional à Escola <b><?=$escola;?></b> e ao Conselho Escolar <b><?=$conselho;?></b>, para 
a implementação do Programa Saúde na Escola, em conformidade com o objetivo maior de contribuir para a formação 
integral dos estudantes por meio de ações de promoção, prevenção e atenção à saúde, no âmbito escolar, com vistas 
ao enfrentamento das vulnerabilidades que comprometem o pleno desenvolvimento de crianças e jovens da rede pública 
de ensino – celebra o presente Termo de Compromisso com o Ministério da Educação, por meio do Programa Saúde 
na Escola, o qual se efetivará por meio de gestão intersetorial entre Educação e Saúde, nos seguintes termos:
<br><br> 
<br><br>

<br class="quebra">

<b>Objeto</b>
<br><br> 
1.	Apoio Institucional, por meio do aporte técnico-formativo e de fortalecimento da articulação e interlocução 
entre a escola e os profissionais das Unidades Básicas de Saúde/ESF - Unidade Local Integrada (ULI), na construção 
da interface educação e saúde, assim como, por meio da distribuição de materiais clínicos e didático-pedagógicos 
à Escola <b><?=$escola;?></b>, por intermédio de seu representante legal, o(a) Diretor(a)<b><?=$diretor;?></b>, 
<br><br>
<br>
<b>Dos Compromissos das Partes</b>
<br><br> 
<b>Artigo I</b>
<br><br>
O Ministério da Educação, por intermédio da Secretaria de Educação Continuada, Alfabetização e Diversidade – 
SECAD/MEC, no âmbito do Programa Saúde na Escola – se compromete a fornecer aporte técnico-formativo e de fortalecimento da articulação e construção da interface educação e saúde, no âmbito escolar, assim como distribuir materiais clínicos e didático-pedagógicos para a Instituição beneficiada mediante a devolução do presente Termo de Compromisso, assinado por seu(s) representante(s) legal(is). 
<br><br> 
<b>Artigo II</b>
<br><br>
Em uma perspectiva intersetorial, a Escola <b><?=$escola;?>/Código INEP nº <?=$inep;?>)</b> se compromete a atuar no 
Território de Responsabilidade, definido segundo a área de abrangência da Estratégia Saúde da Família, ou no 
planejamento integrado com a Unidade Básica de Saúde de referência; tornando possível a aproximação entre os 
sistemas da saúde e da educação favorecendo, por meio da elaboração de planejamento local integrado (Agenda 
Educação e Saúde), estratégias entre a comunidade escolar, a partir de seu projeto político-pedagógico, e a 
Unidade Básica de Saúde/ESF. 
<br><br>
<b>Artigo III</b>
<br><br>
O planejamento das ações do PSE deverá considerar: a gestão democrática, o contexto escolar e social; o diagnóstico 
local e capacidade operativa em saúde do escolar; assim como a parceria com outras áreas como meio ambiente, 
esporte e lazer, cultura, outros.
<br><br>
<b>Artigo IV</b>
<br><br>
As ações a serem implementadas, por meio da gestão intersetorial local entre educação e saúde, deverão contemplar 
os cinco componentes que integram o programa, conforme planejamento local, quais sejam: Avaliação das Condições 
de Saúde dos estudantes de escolas públicas; Promoção da Saúde e Prevenção; Educação Permanente e Capacitação 
dos Profissionais da Educação e da Saúde e de Jovens; Monitoramento e Avaliação da Saúde dos Estudantes e, 
por fim, o componente de Monitoramento e Avaliação do Programa. 
<br><br>
<br class="quebra">
<b>Artigo V</b>
<br><br>
É parte integrante deste Termo de Compromisso, a ser realizado pelas escolas que integram o programa e ora 
assumem o compromisso de sua implementação, o preenchimento no sítio eletrônico http://simec.mec.gov.br, dos 
seguintes formulários de acompanhamento do PSE:
<br><br> 
1.	Cadastro Escola (anual);<br>
2.	Módulo Unidade Local Integrada – ULI (acompanhamento integrado entre Escola/ESF - semestral)<br>
<br><br>
<b>Artigo VI</b>
<br><br>
A escola se compromete a convidar para participação no planejamento e implementação do Programa Saúde na 
Escola seu respectivo Conselho Escolar e o Conselho da Criança e do Adolescente local, quando houver.
<br><br>
<br>
Em nome do Ministério da Educação.
<br><br><br><br>   
_________________________________________________________<br> 
<b>Sr. André Lázaro<br>
Secretário de Educação Continuada, Alfabetização e Diversidade</b><br> 
<br><br><br><br>

Em nome da Escola<br>
<?=$escola;?>/Código INEP nº <?=$inep;?>)<br>
<br><br><br><br>
_________________________________________________________<br> 
Assinatura do(a) Diretor(a)<br>
<br><br><br><br>

Em nome do Conselho Escolar <?=$conselho;?><br>
<br><br><br><br>
_________________________________________________________<br> 
Assinatura do(a) Presidente do Conselho<br>
<br><br><br><br>





______________________/ _____,    ____ de _______________ de 20____.
<br>
(Local/UF)

</p>    
</td>
</tr>
</table>         


</body>