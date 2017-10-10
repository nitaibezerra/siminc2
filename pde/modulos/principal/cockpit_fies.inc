<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
?>
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=9">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>Sistema Integrado de Monitoramento Execução e Controle</title>
    <script language="javascript" type="text/javascript" src="http://portalbi.mec.gov.br/js/jquery.js?1399053977"></script>
    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg')}
    </style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
                <div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_fies&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table class="tabela_painel" align="center" border="0" cellpadding="5" cellspacing="4" width="98%">
<tr>
    <td class="fundo_titulo" style="text-align:center" colspan="4"><div style="margin:28px">FIES Superior</div></td>
</tr>
<tr>
    <td class="fundo_padrao" width="25%">
        <div>
            <div class="titulo_box" style="text-align: center; margin: 5px">ADESÃO</div>
        </div>
    </td>
    <td class="fundo_padrao" colspan="3" width="75%">
        <div>
            <div class="titulo_box" style="text-align: center; margin: 5px">INSCRIÇÃO</div>
        </div>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" rowspan="2">
        <div>
            <?php exibirTitulo('mapas', 'Mantenedoras e<br>IES com adesão<br>ativa ao FIES Superior'); ?>
        </div>
        <div style="text-align: center" id="adesaoBloco">
            <div id="adesaoBloco1">
                <div id="adesaoMantenedoras">Mantenedoras: </div>
                <div id="adesaoMantenedorasNumero"></div>
            </div>
            <div id="adesaoBloco2">
                <div id="adesaoIES">IES: </div>
                <div id="adesaoIESNumero"></div>
            </div>
        </div>
        <div id="adesaoIESUF"></div>
        <div id="adesaoLimite"></div>
    </td>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('doc', 'Contratos formalizados'); ?>
        </div>
        <div id="contratosFormalizados"></div>
        <BR><BR>
        <div>
            <?php exibirTitulo('doc', 'Contratos em preenchimento', '(2013 e 2014)'); ?>
        </div>
        <div id="contratosEmPreenchimento"></div>
    </td>
    <td class="fundo_padrao" colspan="2">
        <div style="text-align: center" id="contratosMapa"></div>
    </td>
</tr>
<tr>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('doc', 'Contratos formalizados<br> por Cor/Raça'); ?>
        </div>
        <div style="text-align: center" id="contratosCor"></div>
    </td>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('doc', 'Contratos formalizados<br> por Sexo'); ?>
        </div>
        <div style="text-align: center" id="contratosSexo"></div>
    </td>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('doc', 'Contratos FGEDUC'); ?>
        </div>
        <div style="text-align: center" id="contratosFGEDUC"></div>
    </td>
</tr>
<tr>
    <td class="fundo_padrao">
        <div>
            <div class="titulo_box" style="text-align: center; margin: 5px">ABATIMENTO 1%</div>
        </div>
        <div>
            <?php exibirTitulo('casas', 'Solicitações Enviadas<br>ao Banco'); ?>
        </div>
        <div></div>
        <div>
            <?php exibirTitulo('casas', 'Solicitações Efetivadas'); ?>
        </div>
        <div></div>
    </td>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('doc', 'Contratos formalizados<br> por nível de Renda'); ?>
        </div>
        <div style="text-align: center" id="contratosRenda"></div>
    </td>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('Preview', 'Cursos mais contratados'); ?>
        </div>
        <div style="text-align: center" id="contratosCurso"></div>
        <div style="text-align: center; margin-top: 20px;" id="contratosGrupo"></div>
    </td>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('Preview', 'Formalização de aditamentos<br> de renovação'); ?>
        </div>
        <div></div>
        <div>
            <?php exibirTitulo('Preview', 'Inscrições em contratação'); ?>
        </div>
        <div></div>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" colspan="3" rowspan="2">
        <div>
            <div class="titulo_box" style="text-align: center; margin: 5px">PAGAMENTO</div>
        </div>
        <div>
            <?php exibirTitulo('financeiro', 'Execução orçamentária'); ?>
        </div>
        <div></div>
        <div>
            <?php exibirTitulo('financeiro', 'Planejamento de Necessidade Orçamentária'); ?>
        </div>
        <div></div>
        <div>
            <?php exibirTitulo('financeiro', 'Volume de Recompras em Reais'); ?>
        </div>
        <div></div>
    </td>
    <!-- Tabela Alinhamento Estratégico -->
    <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',3,60,'','');" align="center" >
        <div>
                <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
                <img src="cockpit/images/alinhamentoEstrategico.png" />
        </div>
    </td>
    <!-- FIM Tabela Alinhamento Estratégico -->
</tr>
<tr>
    <!-- Tabela Processos -->
    <td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/processos/fies/sisfies/default.htm')";>
        <div>
            <?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
        </div>
    </td>
    <!-- Tabela Processos -->
</tr>
</table>
<table class="tabela_painel" align="center" border="0" cellpadding="0" cellspacing="0" width="98%">
    <tr>
        <td class="fundo_td_laranja" colspan="3">
            <div style="text-align:center;" >
                <img src="../imagens/icones/icons/executiverel.png" style="vertical-align:middle;" />
                <input type="text" onclick="this.style.color='#000000';this.value='';" name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;' title='' class=' normal' />
                <img src="../imagens/icones/icons/Find.png" style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);" />
            </div>
        </td>
    </tr>
</table>
<script language="javascript" src="http://portalbi.mec.gov.br/integracoes/simec_fies_inicializacao.js"></script>
</body>
</html>
