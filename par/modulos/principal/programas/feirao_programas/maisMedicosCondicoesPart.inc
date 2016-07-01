<?PHP

if( $_SESSION['par']['muncod'] == '' || $_SESSION['par']['prgid'] == '' ){
	
    echo "
        <script>
                alert('Sessão expirada.');
                window.location.href = 'par.php?modulo=inicio&acao=C';
        </script>
    ";
    die();
}

include_once APPRAIZ . 'includes/cabecalho.inc';
include_once '_funcoes_maismedicos.php';

if($_REQUEST['requisicao'] == 'salvarInfor'){
	global $db; 
    $rqmid = salvarInfor($_POST);
    
    if($rqmid){
    	$_SESSION['par']['rqmid'] = $rqmid;
    	$_SESSION['abaMaisMedicosIM'] = 'S';
    	echo "<script>window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A';</script>";
    } else {
    	$_SESSION['abaMaisMedicosIM'] = 'N';
    	echo "<script>window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosCondicoesPart&acao=A';</script>";
    }
    exit();
}

$rqmid = verificarRQMID();

unset($_SESSION['par']['municipioliberado']);
$municipioLiberado = verificarMunicipioLiberado();

if($rqmid){
	$_SESSION['par']['rqmid'] = $rqmid;
}

if($municipioLiberado){
	$_SESSION['par']['municipioliberado'] = $municipioLiberado;
}

$populacao = verificarPopulacao();
$capital = verificarCapitalEstado();
$capital ? $capital = 't' : $capital = 'f';
$dadosm = verificarDadosMunicipios();

if($dadosm){
	extract($dadosm); 
}

#NÃO POSSUI OFERTA DE CURSO DE MEDICINA EM SEU TERRITÓRIO. 
$oferta = naoPossuiOfertaCurso();

if( $_SESSION['par']['muncod'] != '2601201' ){
    
    if( ($populacao < 70000 || $capital == 't' || $oferta == 'S') ){
        $hab_maismedicos = 'N';
    } else {
        $hab_maismedicos = 'S';
    }
    
}else{
    $hab_maismedicos = 'S';
}

if(empty($_SESSION['par']['rqmid'])){
    unset($_SESSION['abaMaisMedicosIM']);
} else {
    $_SESSION['abaMaisMedicos'] = 'S';
    $_SESSION['abaMaisMedicosIM'] = 'S';
}

if(empty($_SESSION['par']['municipioliberado'])){
	unset($_SESSION['abaMaisMedicosManifestacao']);
} else {
	$_SESSION['abaMaisMedicosManifestacao'] = 'S';
}

echo '<br>';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicosCondicoesPart&acao=A');
monta_titulo($titulo_modulo, recuperaMunicipioEstado());
$_SESSION['maismedicos'] = true;

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']); 
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="javascript">
    function cancelar() {
        window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
    }
</script>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
    <tr class="subtituloEsquerda">
        <td height="10" width="45%">
            <input type="button" value="Voltar" onclick="window.location.href = '?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" id="btnVoltar" />
        </td>
    </tr>
</table>

<br>

<form id="formulario" method="post" name="formulario" action="par.php?modulo=principal/programas/feirao_programas/maisMedicosCondicoesPart&acao=A">
	<input type="hidden" id="requisicao" name="requisicao" value="salvarInfor"/>
	<input type="hidden" id="muncod" name="muncod" value="<?php echo $_SESSION['par']['muncod'];?>"/>
	<input type="hidden" id="prgid" name="prgid" value="<?php echo $_SESSION['par']['prgid']; ?>"/>		
	<input type="hidden" id="rqmquestao03" name="rqmquestao03" value="<?php echo $populacao; ?>"/>
	<input type="hidden" id="rqmquestao04" name="rqmquestao04" value="<?php echo $capital; ?>"/>
	<input type="hidden" id="rqmquestao05" name="rqmquestao05" value="<?php echo $oferta == 'S' ? 't': 'f'; ?>"/>
	<input type="hidden" id="rqmquestao06" name="rqmquestao06" value="<?php echo $leitos_sus; ?>"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	    <tr>
	        <td height="25"></td>
	    </tr>
	    <tr>
	        <td>
                <?php if($hab_maismedicos == 'N'){?>
                	<p style="margin-left: 38%; text-decoration: underline; font-size: 14px; font-weight: bold; color:#FF0000;"><b>MUNICÍPIO NÃO ATENDE AS CONDIÇÕES DE PARTICIPAÇÃO<b></p>
                <?php } ?>
	            <span style="margin-left: 3%; text-decoration: underline; font-size: 14px; font-weight: bold;">
	                CONDIÇÕES DE PARTICIPAÇÃO
	            </span>
	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                Na primeira etapa desta pré-seleção, serão analisadas a relevância e a necessidade social da oferta de curso de medicina no município. O município deverá atender, obrigatoriamente, aos seguintes critérios:
	            </p>
	            <?php
                        if( $_SESSION['par']['muncod'] != '2601201' ){
                            if($populacao < 70000){ 
                    ?>
                                <p style="margin-left: 5%; font-size: 13px; color:#FF0000;"><b>a) ter 70 mil ou mais habitantes, conforme dados do Instituto Brasileiro de Geografia e Estatística (IBGE), Censo 2012.</b></p>    
                    <?      } else { ?>
                                <p style="margin-left: 5%; font-size: 13px;">a) ter 70 mil ou mais habitantes, conforme dados do Instituto Brasileiro de Geografia e Estatística (IBGE), Censo 2012.</p>
                    <?      } 
                        }else{
                    ?>
                                <p style="margin-left: 5%; font-size: 13px;" > a) ter 70 mil ou mais habitantes, conforme dados do Instituto Brasileiro de Geografia e Estatística (IBGE), Censo 2012.</p>
                    <?  } ?>
	            <?php if($capital =='t'){ ?>
                    <p style="margin-left: 5%; font-size: 13px; color:#FF0000;"><b>b) não se constituir capital do Estado.</b></p>
	            <?php } else { ?>
                    <p style="margin-left: 5%; font-size: 13px;">b) não se constituir capital do Estado.</p>
	            <?php } ?>
	            <?php if($oferta == 'S'){ ?>
                    <p style="margin-left: 5%; font-size: 13px; color:#FF0000;"><b> c) não possuir oferta de curso de medicina em seu território. </b></p>
                <?php } else { ?>
                    <p style="margin-left: 5%; font-size: 13px;">c) não possuir oferta de curso de medicina em seu território.</p>
                <?php } ?>
	            <p style="margin-left: 5%; font-size: 13px;">
	                3.2 Os municípios que não atenderem ao disposto nos itens 3.1 e 3.2 serão considerados eliminados da presente pré-seleção.
	            </p>
	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                Na segunda etapa desta pré-seleção, serão analisados a estrutura de equipamentos públicos e os programas de saúde existentes no município, segundo dados do Ministério da Saúde. O município deverá atender, obrigatoriamente, aos seguintes critérios:
	            </p>
	            
	            <p style="margin-left: 5%; font-size: 13px;">
	                a) número de leitos disponíveis SUS por aluno maior ou igual a 5 (cinco), ou seja, para um curso com 50 vagas, o município deverá possuir, no mínimo, 250 leitos disponíveis SUS.
	            <p style="margin-left: 5%; font-size: 13px;">
	                b) número de alunos por equipe de atenção básica menor ou igual a 3 (três), considerando o mínimo de 17 (dezessete) equipes;
	            </p>
	            <p style="margin-left: 5%; font-size: 13px;">
	                c) existência de leitos de urgência e emergência ou Pronto Socorro;                
	            </p>
	            <p style="margin-left: 5%; font-size: 13px;">
	                d) existência de, pelo menos 3 (três), Programas de Residência Médica nas especialidades prioritárias: (1) Clínica Médica; (2) Cirurgia; (3)Ginecologia-Obstetrícia; (4) Pediatria e (5) Medicina de Família e Comunidade;
	            </p>
	            <p style="margin-left: 5%; font-size: 13px;">
	                e) adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica - PMAQ, do Ministério da Saúde;                
	            </p>
	            <p style="margin-left: 5%; font-size: 13px;">
	                f) existência de Centro de Atenção Psicossocial - CAPS;
	            </p>
	            <p style="margin-left: 5%; font-size: 13px;">
	                g) hospital de ensino ou unidade hospitalar com potencial para hospital de ensino, conforme legislação de regência; e
	            </p>
	            <p style="margin-left: 5%; font-size: 13px;">
	                h) existência de hospital com mais de 100 (cem) leitos exclusivos para o curso.
	            </p>
	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                Os municípios que não atenderem ao disposto no item 3.4, de acordo com os dados apresentados pelo próprio município e validados pela SERES - em conformidade com os dados do Ministério da Saúde, ressalvadas as hipóteses dos itens 4.1 a 4.3, serão considerados eliminados da presente pré-seleção.
	            </p>
	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                A terceira etapa da pré-seleção consistirá na análise de projeto de melhoria da estrutura de equipamentos públicos e programas de saúde existentes no município.
	            </p>
	            <p style="margin-left: 5%; text-align: justify; font-size: 13px; font-weight: bold;">
	                Para a realização da terceira etapa desta pré-seleção, a SERES, a seu critério, poderá designar equipes de especialistas para análise de projeto de melhoria da estrutura de equipamentos públicos e programas de saúde no município, assim como para realização de avaliação in loco.
	            </p>
	            <br>
	        </td>
	    </tr>
	    <tr>
	        <td align="center" bgcolor="#CCCCCC" colspan="2">
                <?php if( $hab_maismedicos == 'S' ){ ?>
                <input type="submit" value="Continuar" id="btnContinuar"/>
                <input type="button" value="Cancelar" id="btnCancelar" onclick="cancelar();"/>
                <?php } else { ?>
                <input type="button" value="OK" id="btnCancelar" onclick="cancelar();"/>
                <?php } ?>
	        </td>
	    </tr>						
	</table>
</form>


<script type="text/javascript" language="javascript">
	<?php if($_SESSION['par']['rqmid']){ ?>
		jQuery('#btnContinuar').attr('disabled', 'disabled');
		jQuery('#btnCancelar').attr('disabled', 'disabled');
	<?php } ?>
	<?php if(in_array(PAR_PERFIL_PREFEITO, $arrayPerfil)){ ?>
		jQuery("#btnContinuar").attr('disabled','disabled');
	<?php } ?>	
</script>
