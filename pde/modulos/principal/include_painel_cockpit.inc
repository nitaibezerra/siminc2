<?php

    $perfis = pegaPerfilGeral($_SESSION['usucpf']);

    $acessoEducacaoEspecial = false;
    $acessoPronatec = false;
    $acessoPronacampo = false;
    $acessoBrasilSemMiseria = false;
    $acessoPlanoNacionalEducacao = false;
    $acessoMaisEducacao = false;
    $acessoBrasilProfissionalizado = false;
    $acessoExpansaoEducacaoSuperior = false;
    $acessoConstrucaoCoberturaQuadras = false;
    $acessoBolsaFormacao = false;
    $acessoCienciaSemFronteiras = false;
    $acessoCrechePreEscola = false;
    $acessoExpansaoRedeFederalEpt = false;
    $acessoCaminhoEscola = false;
    $acessoMulheresMil = false;
    $acessoMonitoramentoEnem = false;
    $acessoUniversidadeAberta = false;
    $acessoEducacaoDigital = false;
    $acessoFormacaoProfessores = false;
    $acessoEnsinoMedioInovador = false;
    $acessoPactoAlfabetizacaoIdadeCerta = false;
    $acessoProinfanciaExecucao = false;
    $acessoRedeFederalSuperior = false;
    $acessoInglesSemFronteiras = false;
    $acessoMaisMedicos = false;
    $acessoEducacaoSuperior = false;
    $acessoProuni = false;
    $acessoSisu = false;
    $acessoProinfanciaExecucaoResumo = false;
    $acessoInep = false;
    $acessoFies = false;
    $acessoSEPPIR = false;
    $acessoPosGraduacao = false;
    $acessoBrasilAlfabetizado = false;
    $acessoBolsaPermanencia = false;
    $acessoPDDE = false;
    $acessoOrcamento = false;
    $acessoCienciaSemFronteirasSocio = false;
    $acessoEBSERH = false;
    $acessoPTO = false;
    $acessoPrazosPNE = false;

    //verifica se é adm , su ou consulta e libera acesso pra todos 
    if (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_CONSULTA, $perfis)) {
        $acessoEducacaoEspecial = true;
        $acessoPronatec = true;
        $acessoPronacampo = true;
        $acessoBrasilSemMiseria = true;
        $acessoPlanoNacionalEducacao = true;
        $acessoMaisEducacao = true;
        $acessoBrasilProfissionalizado = true;
        $acessoExpansaoEducacaoSuperior = true;
        $acessoConstrucaoCoberturaQuadras = true;
        $acessoBolsaFormacao = true;
        $acessoCienciaSemFronteiras = true;
        $acessoCrechePreEscola = true;
        $acessoExpansaoRedeFederalEpt = true;
        $acessoCaminhoEscola = true;
        $acessoMulheresMil = true;
        $acessoMonitoramentoEnem = true;
        $acessoUniversidadeAberta = true;
        $acessoEducacaoDigital = true;
        $acessoFormacaoProfessores = true;
        $acessoEnsinoMedioInovador = true;
        $acessoPactoAlfabetizacaoIdadeCerta = true;
        $acessoProinfanciaExecucao = true;
        $acessoRedeFederalSuperior = true;
        $acessoInglesSemFronteiras = true;
        $acessoMaisMedicos = true;
        $acessoEducacaoSuperior = true;
        $acessoProuni = true;
        $acessoSisu = true;
        $acessoProinfanciaExecucaoResumo = true;
        $acessoInep = true;
        $acessoFies = true;
        $acessoSEPPIR = true;
        $acessoPosGraduacao = true;
        $acessoBrasilAlfabetizado = true;
        $acessoBolsaPermanencia = true;
        $acessoPDDE = true;
        $acessoOrcamento = true;
        $acessoCienciaSemFronteirasSocio = true;
        $acessoEBSERH = true;
        $acessoPTO = true;
        $acessoPrazosPNE = true;
    } elseif( in_array(PERFIL_CONSULTA_COCKPIT, $perfis) ) {
        $sql = "
            SELECT c.cocid
            FROM pde.usuarioresponsabilidade ur
            JOIN pde.cockpit c on c.cocid = ur.cocid

            WHERE ur.usucpf = '{$_SESSION['usucpf']}' AND pflcod = ".PERFIL_CONSULTA_COCKPIT." AND rpustatus = 'A'
        ";
        $projetos = $db->carregar($sql);

        if ($projetos) {
            foreach ($projetos as $projeto) {
                switch ($projeto['cocid']) {
                    /*case 1:
                        $acessoBrasilSemMiseria = true;
                        break;*/
                    case 2:
                        $acessoBolsaFormacao = true;
                        break;
                    case 3:
                        $acessoBrasilProfissionalizado = true;
                        break;
                    case 4:
                        $acessoBrasilSemMiseria = true;
                        break;
                    case 5:
                        $acessoCaminhoEscola = true;
                        break;
                    case 6:
                        $acessoCienciaSemFronteiras = true;
                        break;
                    case 7:
                        $acessoConstrucaoCoberturaQuadras = true;
                        break;
                    case 8:
                        $acessoCrechePreEscola = true;
                        break;
                    case 9:
                        $acessoEducacaoDigital = true;
                        break;
                    case 10:
                        $acessoEducacaoSuperior = true;
                        break;
                    case 11:
                        $acessoEnsinoMedioInovador = true;
                        break;
                    /*case 12:
                        $acessoPronacampo = true;
                        break;*/
                    case 13:
                        $acessoExpansaoEducacaoSuperior = true;
                        break;
                    case 14:
                        $acessoRedeFederalSuperior = true;
                        break;
                    case 15:
                        $acessoFormacaoProfessores = true;
                        break;
                    case 16:
                        $acessoInglesSemFronteiras = true;
                        break;
                    case 17:
                        $acessoMaisEducacao = true;
                        break;
                    case 18:
                        $acessoMaisMedicos = true;
                        break;
                    case 19:
                        $acessoMonitoramentoEnem = true;
                        break;
                    case 20:
                        $acessoMonitoramentoEnem = true;
                        break;
                    case 21:
                        $acessoMulheresMil = true;
                        break;
                    case 22:
                        $acessoPactoAlfabetizacaoIdadeCerta = true;
                        break;
                    case 23:
                        $acessoPlanoNacionalEducacao = true;
                        break;
                    case 24:
                        $acessoProinfanciaExecucao = true;
                        break;
                    case 25:
                        $acessoPronacampo = true;
                        break;
                    case 26:
                        $acessoPronatec = true;
                        break;
                    case 27:
                        $acessoProuni = true;
                        break;
                    case 28:
                        $acessoExpansaoRedeFederalEpt = true;
                        break;
                    case 29:
                        $acessoSisu = true;
                        break;
                    case 30:
                        $acessoUniversidadeAberta = true;
                        break;
                    case 31:
                        $acessoEducacaoEspecial = true;
                        break;
					case 32:
                        $acessoProinfanciaExecucaoResumo = true;
                        break;
					case 33:
                        $acessoInep = true;
                        break;
                    case 34:
                        $acessoMonitoramentoEnem = true;
                        break;
                    case 35:
                        $acessoFies = true;
                        break;
                    case 36:
                        $acessoSEPPIR = true;
                        break;
                    case 37:
                        $acessoPosGraduacao = true;
                        break;
                    case 38:
                        $acessoBrasilAlfabetizado = true;
                        break;
                    case 39:
                        $acessoBolsaPermanencia = true;
                        break;
                    case 40:
                        $acessoPDDE = true;
                        break;
                    case 41:
                        $acessoOrcamento = true;
                        break;
                    case 42:
                        $acessoCienciaSemFronteirasSocio = true;
                        break;
                    case 43:
                        $acessoEBSERH = true;
                        break;
                    case 44:
                        $acessoMonitoramentoEnem = true;
                        break;
                    case 45:
                        $acessoPTO = true;
                        break;
                    case 46:
                        $acessoPrazosPNE = true;
                        break;
                }
            }
        }

    } else {
        $sql = "
            select _atiprojeto::integer 
            from pde.responsavelatividade ra
            inner join pde.atividade a on ra.atiid = a.atiid
            inner join entidade.entidade ent on ent.entid = ra.entid
            where ent.entnumcpfcnpj = '{$_SESSION['usucpf']}'
            group by _atiprojeto";
        $projetos = $db->carregar($sql);

        if ($projetos) {
            foreach ($projetos as $projeto) {
                switch ($projeto['_atiprojeto']) {
                    case 131178:
                        $acessoBrasilSemMiseria = true;
                        $acessoEducacaoEspecial = true;
                        $acessoPronatec = true;
                        break;
                    case 129808:
                        $acessoEducacaoEspecial = true;
                        break;
                    case 129708:
                        $acessoEducacaoEspecial = true;
                        break;
                    case 129798:
                        $acessoEducacaoEspecial = true;
                        break;
                    case 129759:
                        $acessoEducacaoEspecial = true;
                        break;
                    case 129716:
                        $acessoEducacaoEspecial = true;
                        break;
                    case 129326:
                        $acessoPronatec = true;
                        break;
                    case 129397:
                        $acessoPronatec = true;
                        $acessoBolsaFormacao = true;
                        break;
                    case 129399:
                        $acessoPronatec = true;
                        break;
                    case 129347:
                        $acessoPronatec = true;
                        break;
                    case 129373:
                        $acessoPronatec = true;
                        $acessoExpansaoRedeFederalEpt = true;
                        break;
                    case 130765:
                        $acessoPronacampo = true;
                        break;
                    case 130761:
                        $acessoPronacampo = true;
                        break;
                    case 130758:
                        $acessoPronacampo = true;
                        $acessoFormacaoProfessores = true;
                        break;
                    case 132193:
                        $acessoPronacampo = true;
                        $acessoFormacaoProfessores = true;
                        break;
                    case 133270:
                        $acessoPronacampo = true;
                        $acessoEducacaoDigital = true;
                        break;
                    case 130762:
                        $acessoPronacampo = true;
                        $acessoFormacaoProfessores = true;
                        break;
                    case 132022:
                        $acessoPronacampo = true;
                        break;
                    case 132034:
                        $acessoPronacampo = true;
                        break;
                    case 130760:
                        $acessoPronacampo = true;
                        break;
                    case 130759:
                        $acessoPronacampo = true;
                        break;
                    case 130766:
                        $acessoPronacampo = true;
                        $acessoCaminhoEscola = true;
                        break;
                    case 63631:
                        $acessoBrasilSemMiseria = true;
                        break;
                    case 129250:
                        $acessoMulheresMil = true;
                        break;
                    case 129399:
                        $acessoBrasilProfissionalizado = true;
                        break;
                    case 129543:
                        $acessoExpansaoEducacaoSuperior = true;
                        break;
                    case 55654:
                        $acessoExpansaoEducacaoSuperior = true;
                        break;
                    case 129209:
                        $acessoExpansaoEducacaoSuperior = true;
                        break;
                    case 129596:
                        $acessoConstrucaoCoberturaQuadras = true;
                        break;
                    case 130531:
                        $acessoCienciaSemFronteiras = true;
                        break;
                    case 129185:
                        $acessoCrechePreEscola = true;
                        break;
                    case 129391:
                        $acessoUniversidadeAberta = true;
                        $acessoFormacaoProfessores = true;
                        break;
                    case 133490:
                        $acessoEducacaoDigital = true;
                        break;
                    case 133996:
                        $acessoFormacaoProfessores = true;
                        break;
                    case 129267:
                        $acessoFormacaoProfessores = true;
                        break;
                    case 129779:
                        $acessoEnsinoMedioInovador = true;
                        break;
                    case 129348:
                        $acessoMaisEducacao = true;
                        break;
                    case 140635:
                        $acessoMaisEducacao = true;
                        break;
                    case 140859:
                        $acessoMaisEducacao = true;
                        break;
                    case 133995:
                        $acessoPactoAlfabetizacaoIdadeCerta = true;
                        break;
                    case 142017:
                        $acessoPactoAlfabetizacaoIdadeCerta = true;
                        break;
                }
            }
        }
    }

//cockpits que nao foram especificados quem pode acessar
//$acessoPlanoNacionalEducacao = true;
//$acessoMonitoramentoEnem = true;
//$acessoProinfanciaExecucao = true;
//$acessoRedeFederalSuperior = true;
//$acessoInglesSemFronteiras = true;
//$acessoMaisMedicos = true;
//$acessoEducacaoSuperior = true;
//$acessoProuni = true;
//$acessoSisu = true;

//Cockpit desativado a pedido da Mariana no dia 25/04/2013
//$acessoPronacampo = false;
//$acessoFormacaoProfessores = false;
    
?>
<link rel="stylesheet" type="text/css" media="all" href="./cockpit/css/include_painel_cockpit.css">

<div class="container">
    <div class="page">
        <div class="section">
            <div class="row">
                <br>
                <div class="content">
                    <div class="agenda">Agendas</div>
                </div>
                <div class="row">
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoEducacaoEspecial ? "window.location.href='estrategico.php?modulo=principal/cockpit_educacao_especial&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoEducacaoEspecial ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_viver_sem_limite.jpg" alt="Educação Especial" width="316" height="67"></div>
                                <div class="txt_programa">Educação Especial</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoPronatec ? "window.location.href='estrategico.php?modulo=principal/cockpit_pronatec&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPronatec ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_pronatec.jpg" alt="Pronatec" width="316" height="67"></div>
                                <div class="txt_programa">Pronatec</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoPronacampo ? "window.location.href='estrategico.php?modulo=principal/cockpit_pronacampo&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPronacampo ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_pronacampo.jpg" alt="Pronacampo" width="316" height="67"></div>
                                <div class="txt_programa">Pronacampo</div>
                            </div>
                        </div>
                    </div>
                    <!-- RETIRADO A PEDIDO DA JAANA DIA 29/12/2014
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoBrasilSemMiseria ? "window.location.href='estrategico.php?modulo=principal/cockpit_brasil_sem_miseria&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoBrasilSemMiseria ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_brasil_sem_miseria.jpg" alt="Brasil sem Miséria" width="316" height="67"></div>
                                <div class="txt_programa">Brasil sem miséria</div>
                            </div>
                        </div>
                    </div>-->
                    <div class="column col1of3">
                        <div class ="content2">
                           <div class="ag_programa" onclick=<?= $acessoSEPPIR ? "window.location.href='estrategico.php?modulo=principal/cockpit_seppir&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoSEPPIR ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_seppir.jpg" alt="SECRETARIA DE POLÍTICAS DE PROMOÇÃO DA IGUALDADE RACIAL" width="316" height="67"></div>
                                <div class="txt_programa">SEPPIR</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoInep ? "window.location.href='estrategico.php?modulo=principal/cockpit_exames_inep&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoInep ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_inep.jpg" alt="Exames e Avaliações do INEP" width="316" height="67"></div>
                                <div class="txt_programa">Exames e Avaliações do INEP</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoOrcamento ? "window.location.href='estrategico.php?modulo=principal/cockpit_orcamento&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoOrcamento ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_orcamento.jpg" alt="Orçamento e Execução" width="316" height="67"></div>
                                <div class="txt_programa">Orçamento e Execução</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class ="content2">
                            <!--<div class="ag_programa" onclick=<?= $acessoPlanoNacionalEducacao ? "window.location.href='estrategico.php?modulo=principal/cockpit_pne&acao=A'" : "" ?> style="cursor:pointer;">-->
                            <div class="ag_programa" onclick=<?= $acessoPlanoNacionalEducacao ? "window.location.href='/pde/graficopne.php'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPlanoNacionalEducacao ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_pne.jpg" alt="Indicadores do Plano Nacional de Educação" width="316" height="67"></div>
                                <div class="txt_programa">Indicadores PNE</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoPTO ? "window.location.href='estrategico.php?modulo=principal/cockpit_pto&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPTO ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_pto.jpg" alt="Projetos do Plano Nacional de Educação" width="316" height="67"></div>
                                <div class="txt_programa">Projetos PNE</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class ="content2">
                            <div class="ag_programa" onclick=<?= $acessoPrazosPNE ? "window.location.href='estrategico.php?modulo=principal/cockpit_prazos_pne&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPrazosPNE ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_prazos.jpg" alt="Prazos do Plano Nacional de Educação" width="316" height="67"></div>
                                <div class="txt_programa">Prazos PNE</div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="column col1of3">
                        <div class="content">
                            <div class="ed_basica">Educação Básica</div>
                            <div class="eb_programa" onclick=<?= $acessoMaisEducacao ? "window.location.href='estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoMaisEducacao ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_mais_educacao.jpg" alt="Mais Educação" width="316" height="67"></div>
                                <div class="txt_programa">Mais Educação</div>
                            </div>
                            <div class="eb_programa" onclick=<?= $acessoConstrucaoCoberturaQuadras ? "window.location.href='estrategico.php?modulo=principal/quadras_estrategico&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoConstrucaoCoberturaQuadras ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_quadras.jpg" alt="Construção e cobertura de quadras esportivas escolares" width="316" height="67"></div>
                                <div class="txt_programa"><font size="-1">Construção e cobertura de quadras esportivas</font></div>
                            </div>
                            <!--<div class="eb_programa" onclick=<?= $acessoCrechePreEscola ? "window.location.href='estrategico.php?modulo=principal/cockpit_estrategico&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoCrechePreEscola ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_pro_infancia.jpg" alt="Creches e Pré-escolas" width="316" height="67"></div>
                                <div class="txt_programa">Creches e Pré-escolas</div>
                            </div>-->
                            <div class="eb_programa" onclick=<?= $acessoProinfanciaExecucao ? "window.location.href='estrategico.php?modulo=principal/cockpit_proinfancia_execucao&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoProinfanciaExecucao ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_obra.jpg" alt="Proinfância Execução" width="316" height="67"></div>
                                <div class="txt_programa">Proinfância Execução</div>
                            </div>
                            <!--<div class="eb_programa" onclick=<?= $acessoProinfanciaExecucaoResumo ? "window.location.href='estrategico.php?modulo=principal/cockpit_proinfancia_execucao_resumo&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoProinfanciaExecucaoResumo ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_obra.jpg" alt="Proinfância Execução" width="316" height="67"></div>
                                <div class="txt_programa">Proinfância Execução Resumo</div>
                            </div>-->
                            <div class="eb_programa" onclick=<?= $acessoCaminhoEscola ? "window.location.href='estrategico.php?modulo=principal/cockpit_caminhos_escola&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoCaminhoEscola ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_caminhos.jpg" alt="Caminho da Escola" width="316" height="67"></div>
                                <div class="txt_programa">Caminho da Escola</div>
                            </div>
                            <div class="eb_programa" onclick=<?= $acessoMonitoramentoEnem ? "window.location.href='estrategico.php?modulo=principal/cockpit_enem2015&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoMonitoramentoEnem ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_enem.jpg" alt="Monitoramento ENEM" width="316" height="67"></div>
                                <div class="txt_programa">Monitoramento ENEM</div>
                            </div>
                            <div class="eb_programa" onclick=<?= $acessoUniversidadeAberta ? "window.location.href='estrategico.php?modulo=principal/cockpit_uab&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoUniversidadeAberta ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_polos.jpg" alt="Universidade Aberta do Brasil - UAB" width="316" height="67"></div>
                                <div class="txt_programa">Universidade Aberta do Brasil - UAB</div>
                            </div>
                            <div class="eb_programa" onclick=<?= $acessoEducacaoDigital ? "window.location.href='estrategico.php?modulo=principal/cockpit_educacao_digital&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoEducacaoDigital ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_educacao_digital.jpg" alt="Educação Digital" width="316" height="67"></div>
                                <div class="txt_programa">Educação Digital</div>
                            </div>
                            <!-- RETIRADO A PEDIDO DA JAANA DIA 29/12/2014
                            <div class="eb_programa" onclick=<?= $acessoFormacaoProfessores ? "window.location.href='estrategico.php?modulo=principal/cockpit_formacao_professores&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoFormacaoProfessores ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_formacao_professores.jpg" alt="Formação de Professores" width="316" height="67"></div>
                                <div class="txt_programa">Formação de Professores</div>
                            </div>
                            -->
                            <div class="eb_programa" onclick=<?= $acessoEnsinoMedioInovador ? "window.location.href='estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoEnsinoMedioInovador ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_ensino_medio_inovador.jpg" alt="Ensino Médio" width="316" height="67" /></div>
                                <div class="txt_programa">Ensino Médio</div>
                            </div>
                            <div class="eb_programa" onclick=<?= $acessoPactoAlfabetizacaoIdadeCerta ? "window.location.href='estrategico.php?modulo=principal/cockpit_pacto&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPactoAlfabetizacaoIdadeCerta ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_pacto.jpg" alt="Pacto pela Alfabetização na Idade Certa" width="316" height="67" /></div>
                                <div class="txt_programa"><font size="-1">Pacto Nacional pela Alfabetização na Idade Certa</font></div>
                            </div>
                            <div class="eb_programa" onclick=<?= $acessoBrasilAlfabetizado ? "window.location.href='estrategico.php?modulo=principal/cockpit_brasil_alfabetizado&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoBrasilAlfabetizado ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_brasilalfabetizado.jpg" alt="Brasil Alfabetizado" width="316" height="67" /></div>
                                <div class="txt_programa">Brasil Alfabetizado</div>
                            </div>
                            <div class="eb_programa" onclick=<?= $acessoPDDE ? "window.location.href='estrategico.php?modulo=principal/cockpit_pdde&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPDDE ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_pdde.jpg" alt="PDDE - Programa Dinheiro Direto na Escola" width="316" height="67" /></div>
                                <div class="txt_programa">PDDE</div>
                            </div>
                        </div>
                    </div>
                    <div class="column col1of3">
                        <div class="content">
                            <div class="ed_profissional">Educação Profissional</div>
                            <div class="ep_programa" onclick=<?= $acessoExpansaoRedeFederalEpt ? "window.location.href='estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoExpansaoRedeFederalEpt ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_expansaorfept.jpg" alt="Expansão da Rede Federal de EPT" width="316" height="67"></div>
                                <div class="txt_programa">Expansão da Rede Federal de EPT</div>
                            </div>
                            <div class="ep_programa" onclick=<?= $acessoBrasilProfissionalizado ? "window.location.href='estrategico.php?modulo=principal/cockpit_brasil_pro&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoBrasilProfissionalizado ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_brasil_profissionalizado.jpg" alt="Brasil Profissionalizado" width="316" height="67"></div>
                                <div class="txt_programa">Brasil Profissionalizado</div>
                            </div>
                            <div class="ep_programa" onclick=<?= $acessoBolsaFormacao ? "window.location.href='estrategico.php?modulo=principal/cockpit_bolsa_formacao&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoBolsaFormacao ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_bolsa_formacao.jpg" alt="Bolsa-Formação" width="316" height="67"></div>
                                <div class="txt_programa">Bolsa-Formação</div>
                            </div>
                            <!--<div class="ep_programa" onclick=<?= $acessoExpansaoRedeFederalEpt ? "window.location.href='estrategico.php?modulo=principal/cockpit_rede_federal&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoExpansaoRedeFederalEpt ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_expansaorfept.jpg" alt="Expansão da Rede Federal de EPT" width="316" height="67"></div>
                                <div class="txt_programa">Expansão da Rede Federal de EPT</div>
                            </div>-->
                            <!-- RETIRADO A PEDIDO DA JAANA DIA 29/12/2014
                            <div class="ep_programa" onclick=<?= $acessoMulheresMil ? "window.location.href='estrategico.php?modulo=principal/cockpit_mulheres_mil&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoMulheresMil ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_mulheres_mil.jpg" alt="Mulheres Mil" width="316" height="67" /></div>
                                <div class="txt_programa">Mulheres Mil</div>
                            </div>
                            -->
                        </div>
                    </div>
                    <div class="column col1of3 lastColumn">
                        <div class="content">
                            <div class="ed_superior">Educação Superior</div>
                            <div class="es_programa" onclick=<?= $acessoEducacaoSuperior ? "window.location.href='estrategico.php?modulo=principal/cockpit_educacao_superior_geral&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoEducacaoSuperior ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_ed_superior_geral.jpg" alt="Expansão da Educação Superior" width="316" height="67"></div>
                                <div class="txt_programa">Expansão da Educação Superior</div>
                            </div>
                            <!--div class="es_programa" onclick=<?= $acessoExpansaoEducacaoSuperior ? "window.location.href='estrategico.php?modulo=principal/cockpit_educacao_superior&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoExpansaoEducacaoSuperior ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_ed_superior.jpg" alt="Expansão da Educação Superior" width="316" height="67"></div>
                                <div class="txt_programa">Expansão da Educação Superior</div>
                            </div-->
                            <div class="es_programa" onclick=<?= $acessoRedeFederalSuperior ? "window.location.href='estrategico.php?modulo=principal/cockpit_rede_federal_es&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoRedeFederalSuperior ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_redefederalsuperior.jpg" alt="Expansão das Universidades Federais" width="316" height="67"></div>
                                <div class="txt_programa"><font size="-1">Expansão das Universidades Federais</font></div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoCienciaSemFronteiras ? "window.location.href='estrategico.php?modulo=principal/cockpit_ciencia_sem_fronteiras&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoCienciaSemFronteiras ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_ciencia_sem_fronteiras.jpg" alt="Ciência sem Fronteiras" width="316" height="67"></div>
                                <div class="txt_programa">Ciência sem Fronteiras</div>
                            </div>
                            <!--<div class="es_programa" onclick=<?= $acessoCienciaSemFronteirasSocio ? "window.location.href='estrategico.php?modulo=principal/cockpit_ciencia_sem_fronteiras_socioeconomico&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoCienciaSemFronteirasSocio ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_ciencia_sem_fronteiras.jpg" alt="Ciência sem Fronteiras Socioeconômico" width="316" height="67"></div>
                                <div class="txt_programa">Ciência sem Fronteiras Socioeconômico</div>
                            </div>-->
                            <div class="es_programa" onclick=<?= $acessoInglesSemFronteiras ? "window.location.href='estrategico.php?modulo=principal/cockpit_ingles_sem_fronteiras&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoInglesSemFronteiras ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_ingles_sem_fronteiras.jpg" alt="Inglês sem Fronteiras" width="316" height="67"></div>
                                <div class="txt_programa">Inglês sem Fronteiras</div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoMaisMedicos ? "window.location.href='estrategico.php?modulo=principal/cockpit_mais_medicos_principal&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoMaisMedicos ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_mais_medicos.jpg" alt="Mais Médicos" width="316" height="67"></div>
                                <div class="txt_programa">Mais Médicos</div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoProuni ? "window.location.href='estrategico.php?modulo=principal/cockpit_prouni&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoProuni ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_prouni.jpg" alt="PROUNI" width="316" height="67"></div>
                                <div class="txt_programa">PROUNI</div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoSisu ? "window.location.href='estrategico.php?modulo=principal/cockpit_sisu&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoSisu ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_sisu.jpg" alt="SISU" width="316" height="67"></div>
                                <div class="txt_programa">SISU</div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoFies ? "window.location.href='estrategico.php?modulo=principal/cockpit_fies&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoFies ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_fies.jpg" alt="FIES - Programa de Financiamento Estudantil" width="316" height="67"></div>
                                <div class="txt_programa">FIES - Programa de Financiamento Estudantil</div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoPosGraduacao ? "window.location.href='estrategico.php?modulo=principal/cockpit_posgraduacao&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoPosGraduacao ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_posgraduacao.jpg" alt="Pós-Graduação" width="316" height="67"></div>
                                <div class="txt_programa">Pós-Graduação</div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoBolsaPermanencia ? "window.location.href='estrategico.php?modulo=principal/cockpit_bolsa_permanencia&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoBolsaPermanencia ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_bolsapermanencia.jpg" alt="Bolsa Permanência" width="316" height="67"></div>
                                <div class="txt_programa">Bolsa Permanência</div>
                            </div>
                            <div class="es_programa" onclick=<?= $acessoEBSERH ? "window.location.href='estrategico.php?modulo=principal/cockpit_ebserh&acao=A'" : "" ?> style="cursor:pointer;">
                                <div class="<?= $acessoEBSERH ? 'img_programa' : 'sem_acesso' ?>"><img src="./cockpit/images/img_ebserh.jpg" alt="EBSERH" width="316" height="67"></div>
                                <div class="txt_programa">EBSERH</div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
		