<!doctype html>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title>Anexo</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<style>
@media print 
{
    .noPrint 
    {
        display:none;
    }
}
</style>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff;">
<div style="text-align:justify;" >
<?php
$uniid = $_GET['uniid'];

if($uniid){
	$sql = "select
				usunome as nome,
				uninome as entnome,
				unicnpj as entnumcpfcnpj,
				mun.mundescricao,
				mun.estuf
			from
				seguranca.usuario usu
			inner join
				maismedicos.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
			inner join
				maismedicos.universidade uni ON uni.uniid = ur.uniid
			inner join
				territorios.municipio mun ON mun.muncod = uni.muncod
			where
				ur.rpustatus = 'A'
			and
				uni.uniid = $uniid";
	$arrDados = $db->pegaLinha($sql);
	$arrReitor = $arrDados;
	if(!$arrDados)
	{
		echo "<script>alert('Favor atribuir o Reitor da Universidade');window.close();</script>";
		exit;
	}
}else{
	echo "<script>alert('Favor informar a Universidade');window.close();</script>";
	exit;
}

?>
<p><b>PROGRAMA MAIS MÉDICOS</b></p>

<p>Termo de Pré-Adesão ao Projeto Mais Médicos</p>

<p>Pelo presente termo a <?php echo $arrDados['entnome'] ?>, com sede no município de <?php echo $arrDados['mundescricao'] ?> - <?php echo $arrDados['estuf'] ?>, inscrita no CNPJ/MF sob o nº <?php echo mascara_global_maismedicos_tela($arrDados['entnumcpfcnpj'],"##.###.###/####-##") ?>, neste ato representada por seu Magnifico (a) Reitor (a) <?php echo $arrReitor['nome'] ?>, doravante, intitulada UNIVERSIDADE manifesta intenção de pré-adesão Projeto Mais Médicos.</p>

<p>Cláusula Primeira - Do Objeto</p>

<p>O presente termo de adesão tem por objeto viabilizar a tutoria e supervisão presencial e a distância de médicos formados em instituições de educação superior brasileiras ou com diploma revalidado no Brasil e médicos formados em instituições de educação superior estrangeiras, por meio de intercâmbio médico internacional inscritos Projeto Mais Médicos, nos termos da Medida Provisória no 621, de 2013, e na Portaria Interministerial MS/MEC no 1.369, de 8 de julho de 2013.</p>

<p>Cláusula Segunda - Das Obrigações</p>

<p>Para consecução do objeto do presente termo a UNIVERSIDADE deverá:</p>

<p>I - Indicar um tutor acadêmico que iniciará suas atividades a partir da assinatura do presente termo e, no mínimo, três tutores que comporão cadastro reserva;</p>
<p>II - Indicar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e supervisão acadêmicas;</p>
<p>III - Cadastrar via sistema <?php echo SIGLA_SISTEMA; ?>, no módulo rede federal, por meio do endereço eletrônico http://simec.mec.gov.br, os tutores indicados e a unidade de avaliação e autorização de pagamento de bolsas;</p>
<p>IV - Definir mecanismo de avaliação e autorização de pagamento das bolsas de tutoria e supervisão.</p>
<p>V - Firmar, em caso de validação, termo de adesão com o Ministério da Educação.</p> 

<p>Cláusula Terceira - Dos Tutores Acadêmicos</p>

<p>I - O Tutor Acadêmico será escolhido pela UNIVERSIDADE dentre os docentes da área médica, vinculados, preferencialmente, a área de conhecimento de saúde coletiva ou correlata ou clínica geral;</p>
<p>II - O tutor acadêmico é responsável pela orientação acadêmica e pelo planejamento das atividades do supervisor;</p>
<p>III - Os tutores do cadastro reserva poderão ser convocados de acordo com o número de médicos selecionados para o programa;</p>
<p>IV - Para o desenvolvimento de suas atividades o tutor acadêmico receberá bolsa-tutoria no valor de R$ 5.000,00 (cinco mil reais).</p>
<p>V - São atribuições do tutor acadêmico, sem prejuízo de outras que vierem a ser definidas pela coordenação do Projeto Mais Médicos:</p>
<p>a) coordenar as atividades acadêmicas da integração ensino-serviço, atuando em cooperação com os supervisores e os gestores do SUS;</p>
<p>b) indicar, em plano de trabalho, as atividades a serem executadas pelos médicos participantes e supervisores e a metodologia de acompanhamento e avaliação;</p>
<p>c) monitorar o processo de acompanhamento e avaliação a ser executado pelos supervisores, garantindo sua continuidade;</p>
<p>d) integrar as atividades do curso de especialização às atividades de integração ensino-serviço;</p>
<p>e) relatar à instituição pública de ensino superior à qual está vinculado a ocorrência de situações nas quais seja necessária a adoção de providência pela instituição; e</p>
<p>f) apresentar relatórios periódicos da execução de suas atividades no Projeto Mais Médicos à instituição pública de ensino superior à qual está vinculado e à Coordenação do Mais Médicos.</p>

<p><?php echo $arrDados['mundescricao'] ?> - <?php echo $arrDados['estuf'] ?> / <?php echo date("d/m/Y") ?></p>

<p>_____________________________________________</p>
<p><?php echo $arrReitor['nome'] ?></p>
</div>
<div class="noPrint">
	<input type="button" name="btn_imprimir" value="Imprimir" onclick="window.print()" />
	<input type="button" name="btn_fechar" value="Fechar" onclick="window.close()" />
</div>
</body>
</html>