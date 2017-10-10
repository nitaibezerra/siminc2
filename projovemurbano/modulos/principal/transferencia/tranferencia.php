<?php

    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }

    if ($_REQUEST['estuf']){
        $_SESSION['projovemurbano']['estuf'] = $_REQUEST['estuf'];
    } else if ($_REQUEST['muncod']){
        $_SESSION['projovemurbano']['muncod'] = $_REQUEST['muncod'];
    }

    if (!$_SESSION['projovemurbano']['pjuid']) {
        carregarProJovemUrbano();
        //encaminharUltimoAcesso();
    }

    if (!$_SESSION['projovemurbano']['pjuid']) {
        die("<script>
                                alert('Problema encontrado no carregamento. Inicie novamente a navegação.');
                                window.location='projovemurbano.php?modulo=inicio&acao=C';
                         </script>");
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Urbano', montaTituloEstMun());

    
    if( $_SESSION['projovemurbano']['ppuid'] == '1' ){
        echo intrucaoAno_2012();
    }elseif( $_SESSION['projovemurbano']['ppuid'] == '2' ){
        echo intrucaoAno_2013();
    } 
?>

<?php
function intrucaoAno_2012(){
    
    if ($_REQUEST['estuf'])
        $strH2 = "Senhor Secretário Estadual de Educação";
    else if ($_REQUEST['muncod'])
       $strH2 = "Senhor Secretário Municípal de Educação";
?>
    <form id="form" name="form" method="POST">

        <center>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                <tr>
                    <td width="90%" align="center">
                        <div style="overflow: auto; height:380px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid; text-align:left; padding: 10 10 10 10;" >
                            <h1>Instruções</h1>
                            <? if ($_SESSION['projovemurbano']['estuf']) : ?>

                                <h2><?php echo $strH2 ?>,</h2>

                                <p>Caso seja do interesse desse estado fazer a adesão ao Programa Projovem Urbano é necessário:</p>


                                <p>De 11 de novembro a 1º de dezembro de 2011:</p>
                                <p>1.	Preencher e validar este Termo de Adesão no qual está indicada a meta de atendimento proposta para seu estado;</p>
                                <p>2.	Preencher com sua sugestão de meta o campo disponível que pode ser maior ou menor daquela já indicada.</p>
                                <p>A proposta de meta do termo de adesão, firmado no prazo estabelecido acima, será analisada pela SECADI e a meta de seu estado poderá ser ajustada</p>


                                <p>De 05 a 16 de dezembro de 2011:</p>
                                <p>1.	Visualizar o Termo de Adesão com os ajustes neste sistema;</p>
                                <p>2.	Validar novamente a sugestão do estado, caso ela tenha sido aprovada pela SECADI;</p>
                                <p>3.	Validar, também, caso a sugestão não tenha sido aprovada, ou seja, diferente da meta original.</p>


                                <p>De 19 de dezembro de 2011 a 16 de janeiro de 2012:</p>
                                <p>1.	Indicar um Coordenador Geral do Projovem Urbano deste estado, escolhido entre os profissionais do quadro efetivo da Secretária de Educação ou selecionado e contratado com recursos próprios (ver <a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolução CD/FNDE Nº 60, de 9 de novembro de 2011</a>);</p>
                                <p>2.	Solicitar o cadastro do Coordenador Geral  no módulo Projovem Urbano do <?php echo SIGLA_SISTEMA; ?> para o preenchimento do Plano de Implementação, conforme as instruções do <?php echo SIGLA_SISTEMA; ?> e as determinações da Resolução n° /2011 – <a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolução CD/FNDE Nº 60, de 9 de novembro de 2011</a>;</p>
                                <p>3.	Analisar e validar o Plano de Implementação que deverá estar devidamente preenchido e finalizado para a análise da SECADI;</p>
                                <p>4.	Imprimir e assinar o Plano de Implementação, após a aprovação desta secretaria, e enviá-lo para o endereço:</p>
                                <p>
                                    Secretaria de Educação Continuada, Alfabetização e Inclusão.<br>
                                    Projovem Urbano 2012<br>
                                    Ministério da Educação<br>
                                    Esplanada do Ministério<br>
                                    Bloco L – 2º andar – sala 220<br>
                                    Brasília - DF<br>
                                    Cep 70.047-900
                                </p>
                            <? endif; ?>

                            <? if ($_SESSION['projovemurbano']['muncod']) : ?>

                                <h2>Senhor Secretário Municipal de Educação,</h2>

                                <p>Caso seja do interesse desse município fazer a adesão ao Programa Projovem Urbano é necessário:</p>


                                <p>De 11 de novembro a 1º de dezembro de 2011:</p>
                                <p>1.	Preencher e validar este Termo de Adesão no qual está indicada a meta de atendimento proposta por seu município;</p>
                                <p>2.	Preencher com sua sugestão de meta o campo disponível que pode ser maior ou menor daquela já indicada.</p>
                                <p>A proposta de meta do termo de adesão, firmado no prazo estabelecido acima, será analisada pela SECADI e a meta de seu município poderá ser ajustada.</p>


                                <p>De 05 a 16 de dezembro de 2011:</p>
                                <p>1.	Visualizar o Termo de Adesão com os ajustes neste sistema;</p>
                                <p>2.	Validar novamente a sugestão do município, caso ela tenha sido aprovada pela SECADI;</p>
                                <p>3.	Validar, também, caso a sugestão não tenha sido aprovada, ou seja, diferente da meta original.</p>


                                <p>De 19 de dezembro de 2011 a 16 de janeiro de 2012:</p>
                                <p>1.	Indicar um Coordenador Geral do Projovem Urbano deste município, escolhido entre os profissionais do quadro efetivo da Secretária de Educação ou selecionado e contratado com recursos próprios (<a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolução CD/FNDE Nº 60, de 9 de novembro de 2011</a>);</p>
                                <p>2.	Solicitar o cadastro do Coordenador Geral no módulo Projovem Urbano do <?php echo SIGLA_SISTEMA; ?> para o preenchimento do Plano de Implementação, conforme as instruções do <?php echo SIGLA_SISTEMA; ?> e as determinações da Resolução n° /2011 – <a href="http://www.fnde.gov.br/index.php/legis-resolucoes" target="_blank">Resolução CD/FNDE Nº 60, de 9 de novembro de 2011</a>;</p>
                                <p>3.	Analisar e validar o Plano de Implementação que deverá estar devidamente preenchido e finalizado para a análise da SECADI;</p>
                                <p>4.	Imprimir e assinar o Plano de Implementação, após a aprovação desta secretaria, e enviá-lo para o endereço:</p>
                                <p>
                                    Secretaria de Educação Continuada, Alfabetização e Inclusão.<br/>
                                    Projovem Urbano 2012<br/>
                                    Ministério da Educação<br/>
                                    Esplanada do Ministério<br/>
                                    Bloco L – 2º andar – sala 220<br/>
                                    Brasília - DF<br/>
                                    Cep 70.047-900
                                <p>
                                <? endif; ?>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloCentro"><input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/identificacao&acao=A';"></td>
                </tr>
            </table>
        </center>
    </form>
<?php
}


function intrucaoAno_2013(){
    
    if ($_REQUEST['estuf'])
        $strH2 = "Senhor Secretário Estadual de Educação";
    else if ($_REQUEST['muncod'])
        $strH2 = "Senhor Secretário Municípal de Educação";
?>
<form id="form" name="form" method="POST">
	<center>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td width="90%" align="center">
				<div style="overflow: auto; height:380px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid; text-align:left; padding: 10 10 10 10;" >
					<h1>Instruções</h1>
					
					<h2><?php echo $strH2 ?>,</h2>

						<p>A adesão ao Projovem Urbano para a edição 2013 foi pactuada com os Senhores, 
                        	por meio de Termo de Adesão anexado à mensagem eletrônica e posteriormente encaminhado a esta DPEJUV/MEC. 
                        	Agora esta adesão deverá ser formalizada através do <?php echo SIGLA_SISTEMA; ?> - módulo Projovem Urbano.</p>

                       	<p>Para tanto, deverão ser adotados os seguintes procedimentos:</p>
                                
                       	<p>1. Preencher os campos na aba - Identificação com os dados do Senhor(a) Secretário(a).</p>
                       	<p>2. Validar/aceitar o Termo de Adesão no qual está indicada a meta de atendimento no seu estado/município.</p>
                       	<p>
                        	3. Imprimir e assinar o Termo de Adesão e enviá-lo para o seguinte endereço:
                               Secretaria de Educação Continuada, Alfabetização e Inclusão - SECADI
                               Diretoria de Políticas de Educação para a Juventude - DPEJUV
                               Ministério da Educação
                               Esplanada dos Ministérios - Bloco L - 2º andar - Sala 220
                               CEP 70.047-900 Brasília-DF
                        </p>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro"><input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/identificacao&acao=A';"></td>
			</tr>
		</table>
	</center>
</form>
<?
}
?>
<? registarUltimoAcesso(); ?>
