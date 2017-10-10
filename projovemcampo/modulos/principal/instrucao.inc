<?php

    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }

    if ($_REQUEST['estuf']){
        $_SESSION['projovemcampo']['estuf'] = $_REQUEST['estuf'];
    } else if ($_REQUEST['muncod']){
        $_SESSION['projovemcampo']['muncod'] = $_REQUEST['muncod'];
    }

	carregarProjovemCampo();

    if (!$_SESSION['projovemcampo']['apcid']) {
        die("<script>
                                alert('Problema encontrado no carregamento. Inicie novamente a navegação.');
                                window.location='projovemcampo.php?modulo=inicio&acao=C';
                         </script>");
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    echo montarAbasArray(montaMenuProJovemCampo(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Campo', montaTituloEstMun());

    
    echo intrucao();


function intrucao(){
	
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
					
					 <p>Senhor(a) Secretário(a) de Educação,<p>
<br />
                                <p>Caso seja do interesse dessa localidade fazer a adesão ao Programa Projovem Campo é necessário:</p>
<br />
								<p>-	Aceitar este Termo de Adesão no qual está indicada a meta de atendimento proposta para essa localidade.
                                <p>-	Preencher com sua sugestão de meta o campo disponível nesse Sistema que pode ser maior ou menor daquela já indicada. A proposta de meta do termo de adesão será analisada pela DPEJUV/SECADI e a meta poderá ser ajustada ou não.</p>
                                <p>-    Visualizar o Termo de Adesão com os ajustes neste sistema e ACEITAR novamente o Termo de Adesão com a meta ajustada, caso ela tenha sido aprovada pela DPEJUV/SECADI.</p>
								<p>-    ACEITAR, também, caso a sugestão de meta não tenha sido aprovada
								<p>-    Imprimir e assinar o Termo de Adesão, após o ACEITE final por essa Secretaria, e enviar para o seguinte endereço:
                               <p> 
                                	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ministério da Educação – MEC <br />
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Secretaria de Educação Continuada, Alfabetização e Inclusão - SECADI.<br/>
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Diretoria de Políticas de Educação para a Juventude - DPEJUV<br/>
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Esplanada dos Ministérios, Bloco L – 2º andar – Sala 220<br/>
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;CEP: 70.047-900  Brasília-DF<br/>
                                <p>
                                
                                <p>-  Indicar o Coordenador Geral do Programa dessa localidade, escolhido entre os profissionais do quadro efetivo da Secretária de Educação ou selecionado/contratado (Resolução CD/FNDE/MEC Nº 11, de 16 de abril de 2014).
					            <p>-  Solicitar o cadastro do Coordenador Geral no módulo Projovem Urbano do <?php echo SIGLA_SISTEMA; ?> para o preenchimento do Plano de Implementação, conforme as instruções do <?php echo SIGLA_SISTEMA; ?> e as determinações da Resolução CD/FNDE/MEC Nº 11, de 16 de abril de 2014
					            <p>-  Analisar e enviar o Plano de Implementação que deverá estar devidamente preenchido e finalizado para a análise da DPEJUV/SECADI.
					            <p>-  Imprimir e assinar o Plano de Implementação, após a validação desta Diretoria, e enviá-lo para o seguinte endereço:
					           <p> 
                                	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ministério da Educação – MEC <br />
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Secretaria de Educação Continuada, Alfabetização e Inclusão - SECADI.<br/>
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Diretoria de Políticas de Educação para a Juventude - DPEJUV<br/>
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Esplanada dos Ministérios, Bloco L – 2º andar – Sala 220<br/>
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;CEP: 70.047-900  Brasília-DF<br/>
                                <p>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro"><input type="button" name="proximo" value="Próximo" onclick="window.location='projovemcampo.php?modulo=principal/identificacao&acao=A';"></td>
			</tr>
		</table>
	</center>
</form>
<?
}
registarUltimoAcesso();
?>