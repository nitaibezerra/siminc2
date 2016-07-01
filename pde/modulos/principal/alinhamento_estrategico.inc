<?php
if($_REQUEST['requisicao'] == 'verificarPerfilParaAcessoModulo') {
    $_REQUEST['requisicao']($_REQUEST);
}

if ($_REQUEST['requisicao'] == 'carregaRoll') {
    switch ($_REQUEST['area']) {
        case 'pe':
            $sql = "select opeid as codigo, openome as descricao from pde.ae_objetivope
					where temid = {$_REQUEST['temid']} order by openome";
            $subTitulo = "Objetivos Estratégicos";
            break;
        case 'obj':
            $sql = "select obeid as codigo, obenome as descricao from pde.ae_objetivoestrategico
					where temid = {$_REQUEST['temid']} order by obenome";
            $subTitulo = "Objetivos Estratégicos";
            break;
        case 'ppa':
            $sql = "select objid as codigo, objnome as descricao from pde.ae_objetivoppa
					where temid = {$_REQUEST['temid']} order by objnome";
            $subTitulo = "Objetivos";
            break;
        case 'pne':
			$sql = "select
						m.mpneid as codigo, 
						coalesce(ind.link,'') || m.mpnenome as descricao
					from pde.ae_metapne m
					left join
						(
						select
							'<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
							mpneid
						from pde.ae_metapnexindicador group by mpneid
						) as ind ON ind.mpneid = m.mpneid
					where m.temid = {$_REQUEST['temid']}
					and m.mpneid <> 99999999
					order by m.mpneordem";
            $subTitulo = "Metas";
            break;
        case 'ae':
            $sql = "select acaid as codigo, acadsc as descricao from painel.acao
				where acastatus = 'A' AND temid = {$_REQUEST['temid']}  order by acadsc ";
            $subTitulo = "Ações";
            break;
        case 'orc':
			$sql = "SELECT DISTINCT sba.codigo AS codigo, sba.codigo || ' - ' || sba.titulo AS descricao
					FROM planacomorc.vinculacaoestrategicasubacoes ves
					INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
					INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
					INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
					INNER JOIN painel.acao aca ON aca.acaid = vac.acaid
					WHERE aca.temid = {$_REQUEST['temid']}
					ORDER BY descricao";
            $subTitulo = "Subações Orçamentárias";
            break;
        case 'pro':
            $sql = "SELECT
                        DISTINCT sol.solid AS codigo,
                        '<a href=''/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='|| sol.solid ||''' target=''_blank''><img src=''../imagens/icones/icons/Preview.png'' height=''30px'' width=''30px'' title=''Detalhar Projeto''></a>&nbsp;' || COALESCE(sol.solapelido,sol.soldsc) AS descricao,
                        sol.solordem
                    FROM pto.solucao sol
                    INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                    WHERE sol.solstatus = 'A'
                    AND tes.temid = {$_REQUEST['temid']}
                    ORDER BY sol.solordem";
            $subTitulo = "Projetos";
            break;
    }
    if ($sql) {
        $rs = $db->carregar($sql);
        echo '<center><h2><p><b>' . $subTitulo . '</b></p></h2></center>';
        if ($rs) {
            echo '<div id="div_lnk_objetivos_metas">';
            foreach ($rs as $dados) {
                echo '<div id="' . $_REQUEST['area'] . '_' . $dados['codigo'] . '"  class="objetivos_metas ' . ($_REQUEST['classe'] ? $_REQUEST['classe'] : 'verde') . '" style="margin-bottom: 5px; cursor: pointer;"> ' . $dados['descricao'] . '</div>';
            }
            echo '</div>';
        }
    } else {
        echo "Sem registros.";
    }
    die;
}

if ($_REQUEST['requisicao'] == 'carregaTable') {

    $arId = explode('_', $_REQUEST['id']);
    if (is_array($arId)) {
        switch ($arId[0]) {
            case 'pne':
                montaGridPNE();
                break;
            case 'ppa':
                montaGridPPA();
                break;
            case 'pe':
                montaGridPE();
                break;
            case 'obj':
                montaGridOBJ();
                break;
            case 'ae':
                montaGridAE();
                break;
            case 'orc':
                montaGridORC();
                break;
            case 'pro':
                montaGridPRO();
                break;
        }
    }
    die;
}

if ($_REQUEST['requisicao'] == 'carregaMetasPNE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select mpneid from pde.ae_desafioxmetapne where desid = {$_REQUEST['id']}";
        $dados = $db->carregarColuna($sql, 'mpneid');
        $metas = implode(',', $dados);

        if ($metas) {
			$sql = "select 
						m.temid, 
						m.mpneid as codigo, 
						coalesce(ind.link,'') || m.mpnenome as nome 
					from pde.ae_metapne m
					left join
						(
						select
							'<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
							mpneid
						from pde.ae_metapnexindicador group by mpneid
						) as ind ON ind.mpneid = m.mpneid
					where m.mpneid in ($metas)
					and m.mpneid <> 99999999
					order by m.mpneordem";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 verde clickMetasPNE" style="margin:0 0 0 0;">
                            <span class="textoDescricao">' . $data['nome'] . '</span><br/>
			            </div>
                    ';
                }
            }
        }
    }
    die;
}

if ($_REQUEST['requisicao'] == 'carregaMetasPPAPE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select mppaid from pde.ae_desafioxmetappa where desid = {$_REQUEST['id']}";
        $dados = $db->carregarColuna($sql, 'mppaid');
        $metas = implode(',', $dados);

        if ($metas) {
            $sql = "
                select mppaid as codigo, mppanome as nome, temid, o.objid, mppatipo from pde.ae_metappa m
                inner join pde.ae_objetivoppa o on m.objid = o.objid
                where mppaid in ($metas) order by mppanome
            ";
            $rs = $db->carregar($sql);

            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['objid'] . '_' . $data['codigo'] . '_' . $data['mppatipo'] . '"  class="tile1 tile4 azul clickMetasPPA">
                            <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
			            </div>
                    ';
                }
            }
        }
    }
    die;
}


if($_REQUEST['requisicao'] == 'carregaDesafiosPE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select desid from pde.ae_desafioxmetappa where mppaid =" . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'desid');
        $desafios = implode(',', $dados);
        if ($desafios) {
            $sql = "select desid as codigo, desnome as nome, temid, o.opeid from pde.ae_desafio d
			inner join pde.ae_objetivope o on d.opeid = o.opeid where desid in ($desafios) order by desnome";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['opeid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickDesafiosPE">
                            <span class="textoDescricao">' . $data['nome'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

if($_REQUEST['requisicao'] == 'carregaObjetivosEstrategicosPE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select obeid from pde.ae_objetivoestrategicoxmetappa where mppaid =" . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'obeid');
        $objetivos = implode(',', $dados);
        if ($objetivos) {
            $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($objetivos) order by obenome";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickObjetivoEstrategico">
                            <span class="textoDescricao">' . $data['nome'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

if($_REQUEST['requisicao'] == 'carregaMetasPNEPPA') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select mpneid from pde.ae_metapnexppa where mppaid = {$_REQUEST['id']}";
        $dados = $db->carregarColuna($sql, 'mpneid');
        $metas = implode(',', $dados);

        if ($metas) {
			$sql = "select 
						m.temid, 
						m.mpneid as codigo, 
						coalesce(ind.link,'') || m.mpnenome as nome 
					from pde.ae_metapne m
					left join
						(
						select
							'<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
							mpneid
						from pde.ae_metapnexindicador group by mpneid
						) as ind ON ind.mpneid = m.mpneid
					where m.mpneid in ($metas)
					and m.mpneid <> 99999999
					order by m.mpneordem";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 verde clickMetasPNE">
                            <span class="textoDescricao">' . $data['nome'] . '</span><br/>
			            </div>
                    ';
                }
            }
        }
    }
    die;
}

if ($_REQUEST['requisicao'] == 'carregaMetasAcoesPE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select acaid from pde.ae_acaoxdesafio where desid = {$_REQUEST['id']}";
        $dados = $db->carregarColuna($sql, 'acaid');
        $acoes = implode(',', $dados);

        if ($acoes) {
            $sql = "
                select aca.temid, aca.acaid, aca.acadsc, aca.acadetalhe, coc.coclink, aca.indidprincipal
                from painel.acao aca
                left join pde.cockpit coc ON coc.cocid = aca.cocid
                where aca.acaid in ($acoes)
                order by aca.acadsc
            ";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['acaid'] . '" >
                            ' . ($data['coclink'] ? '<a href="' . $data['coclink'] . '" class="linkPopupPainel" target="_blank" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '') . '
                            ' . ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid=' . $data['indidprincipal'] . '" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '') . '
                            <a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid=' . $data['acaid'] . '" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;
                            <span class="textoDescricao" title="' . $data['acadetalhe'] . '" alt="' . $data['acadetalhe'] . '">' . $data['acadsc'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

if ($_REQUEST['requisicao'] == 'carregaAcoesPEPPA') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select acaid from pde.ae_acaoxmetappa where mppaid = {$_REQUEST['id']}";
        $dados = $db->carregarColuna($sql, 'acaid');
        $acoes = implode(',', $dados);

        if ($acoes) {
            $sql = "
                select aca.temid, aca.acaid, aca.indidprincipal, aca.acadsc, aca.acadetalhe, coc.coclink
                from painel.acao aca
                left join pde.cockpit coc ON coc.cocid = aca.cocid
                where aca.acaid in ($acoes)
                order by aca.acadsc
            ";
            $rs = $db->carregar($sql);

            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['acaid'] . '" >
                            ' . ($data['coclink'] ? '<a href="' . $data['coclink'] . '" target="_blank" class="linkPopupPainel" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '') . '
                            ' . ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid=' . $data['indidprincipal'] . '" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '') . '
                            <a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid=' . $data['acaid'] . '" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;
                            <span class="textoDescricao" title="' . $data['acadetalhe'] . '" alt="' . $data['acadetalhe'] . '">' . $data['acadsc'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

if($_REQUEST['requisicao'] == 'carregaAcoesPDE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select acaid from pde.ae_acaoxmetapne where mpneid =" . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'acaid');
        $acoes = implode(',', $dados);

        if ($acoes) {
            $sql = "
                select aca.temid, aca.acaid as codigo, aca.acadsc as nome, aca.acadetalhe, aca.indidprincipal, coc.coclink
                from painel.acao aca
                left join pde.cockpit coc ON coc.cocid = aca.cocid
                where aca.acaid in ($acoes)
                order by aca.acadsc
            ";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['codigo'] . '" >
                            ' . ($data['coclink'] ? '<a href="' . $data['coclink'] . '" target="_blank" class="linkPopupPainel" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '') . '
                            ' . ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid=' . $data['indidprincipal'] . '" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '') . '
                            <a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid=' . $data['codigo'] . '" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;
                            <span class="textoDescricao" title="' . $data['acadetalhe'] . '" alt="' . $data['acadetalhe'] . '">' . $data['nome'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

if($_REQUEST['requisicao'] == 'carregarDesafiosPNE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select desid from pde.ae_desafioxmetapne where mpneid = " . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'desid');
        $desafios = implode(',', $dados);

        if ($desafios) {
            $sql = "select desid as codigo, desnome as nome, temid, o.opeid from pde.ae_desafio d
			inner join pde.ae_objetivope o on d.opeid = o.opeid where desid in ($desafios) order by desnome";
            $rs = $db->carregar($sql);

            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['opeid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickDesafiosPE">
                            <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
			            </div>
                    ';
                }
            }
        }
    }
    die;
}

if($_REQUEST['requisicao'] == 'carregarObjetivosEstrategicosPNE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select obeid from pde.ae_objetivoestrategicoxmetapne where mpneid = " . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'obeid');
        $objetivos = implode(',', $dados);

        if ($objetivos) {
            $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($objetivos) order by obenome";
            $rs = $db->carregar($sql);

            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickObjetivoEstrategico">
                            <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
			            </div>
                    ';
                }
            }
        }
    }
    die;
}

if($_REQUEST['requisicao'] == 'carregaProjetosPNE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select solid from pto.metasolucao where mpneid = " . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'solid');
        $solucoes = implode(',', $dados);

        if ($solucoes) {
            $sql = "SELECT DISTINCT tes.temid, sol.solid AS codigo, COALESCE(sol.solapelido,sol.soldsc) AS nome, sol.solordem
                    FROM pto.solucao sol
                    INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                    WHERE sol.solstatus = 'A'
                    AND sol.solid IN ($solucoes)
                    ORDER BY sol.solordem";
            $rs = $db->carregar($sql);

            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 marromClaro clickProjetos">
                            <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
                            <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
			            </div>
                    ';
                }
            }
        }
    }
    die;
}

if($_REQUEST['requisicao'] == 'carregaProjetosPPA') {
    if ($_REQUEST['id'] > 0) {
        $sql = "SELECT DISTINCT acs.solid
                FROM pto.metasolucao acs
                INNER JOIN pde.ae_metapnexppa app ON app.mpneid = acs.mpneid
                WHERE app.mppaid  = " . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'solid');
        $solucoes = implode(',', $dados);

        if ($solucoes) {
            $sql = "SELECT DISTINCT tes.temid, sol.solid AS codigo, COALESCE(sol.solapelido,sol.soldsc) AS nome, sol.solordem
                    FROM pto.solucao sol
                    INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                    WHERE sol.solstatus = 'A'
                    AND sol.solid IN ($solucoes)
                    ORDER BY sol.solordem";
            $rs = $db->carregar($sql);

            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 marromClaro clickProjetos">
                            <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
                            <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
			            </div>
                    ';
                }
            }
        }
    }
    die;
}

if ($_REQUEST['requisicao'] == 'carregaMetaPPA') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select mppaid from pde.ae_metapnexppa where mpneid = {$_REQUEST['id']}";
        $dados = $db->carregarColuna($sql, 'mppaid');
        $metas = implode(',', $dados);

        if ($metas) {
            $sql = "
                select mppaid as codigo, mppanome as nome, temid,o.objid, mppatipo from pde.ae_metappa
                m  inner join pde.ae_objetivoppa o on m.objid = o.objid
                where mppaid in ($metas) order by mppanome
            ";
            $rs = $db->carregar($sql);

            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div id="' . $data['temid'] . '_' . $data['objid'] . '_' . $data['codigo'] . '_' . $data['mppatipo'] . '"  class="tile1 tile4 azul clickMetasPPA">
                            <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}


//ORÇAMENTO PLANO NACIONAL DE EDUCAÇÃO
if  ($_REQUEST['requisicao'] == 'carregaOrcamentos') {
    if ($_REQUEST['id'] > 0) {
 		$sql = "select acaid from pde.ae_acaoxmetapne where mpneid =" . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'acaid');
        $orcamentos = implode(',', $dados);

        if ($orcamentos) {
			$sql = "SELECT vacid, vaetituloorcamentario AS nome
					FROM planacomorc.vinculacaoacaoestrategica
					WHERE acaid in ($orcamentos)
					ORDER BY nome";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');">
							<span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

//ORÇAMENTO PLANO PLURIANUAL
if ($_REQUEST['requisicao'] == 'carregaOrcamentosPPA') {
    if ($_REQUEST['id'] > 0) {
		$sql = "select acaid from pde.ae_acaoxmetappa where mppaid =" . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'acaid');
        $orcamentos = implode(',', $dados);

        if ($orcamentos) {
            $sql = "SELECT vacid, vaetituloorcamentario AS nome
					FROM planacomorc.vinculacaoacaoestrategica
					WHERE acaid in ($orcamentos)
					ORDER BY nome";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');">
							<span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

//ORÇAMENTO OBJETIVOS ESTRATÉGICOS E DESAFIOS
if ($_REQUEST['requisicao'] == 'carregaOrcamentosPE') {
    if ($_REQUEST['id'] > 0) {
		$sql = "select acaid from pde.ae_acaoxdesafio where desid =" . $_REQUEST['id'];
        $dados = $db->carregarColuna($sql, 'acaid');
        $orcamentos = implode(',', $dados);

        if ($orcamentos) {
            $sql = "SELECT vacid, vaetituloorcamentario AS nome
					FROM planacomorc.vinculacaoacaoestrategica
					WHERE acaid in ($orcamentos)
					ORDER BY nome";
            $rs = $db->carregar($sql);
            if ($rs) {
                foreach ($rs as $data) {
                    echo '
                        <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');">
							<span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                        </div>
                    ';
                }
            }
        }
    }
    die;
}

if ($_REQUEST['requisicao'] == 'carregaEstrategiasPNE') {
    if ($_REQUEST['id'] > 0) {
        $sql = "select estnome as nome from pde.ae_estrategia where metid = " . $_REQUEST['id'] . " ORDER BY estordem";
        $rs = $db->carregar($sql);

        if ($rs) {
            foreach ($rs as $data) {
                echo '
                    <div class="tile1 tile4 azulClaro" style="cursor: default">
                        <span class="textoDescricao">' . $data['nome'] . '</span><br/>
                    </div>
                ';
            }
        }
    }
    die;
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">

    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel="stylesheet" type="text/css" href="css/jit_win8_desafios.css"/>

    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script type="text/javascript" src="js/jit_win8_desafios.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/Jit/jit.js"></script>

    <div id="conteudotip" style="display:none">Você poderá pesquisar as iniciativas do Ministério da Educação por: Estados, Municípios, Universidades, Escolas, Regiões, Territórios, Ações, Programas, Indicadores, Secretarias, Autarquias, etc.</div>

    <style type="text/css">
        html {
            background: url("cockpit/images/bg_mon.jpg") repeat-x scroll 0 28px #010206;
            color: #333333;
            padding: 0;
        }

        #busca{
            background: none repeat scroll 0% 0% rgb(255, 255, 255);
            width:400px;
            border-width: 1px;
            border-style: solid;
            border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204);
            color: rgb(0, 0, 0);
            font: 18px arial,sans-serif bold;
            height: 35px;
        }
        .fundo_td_laranja{
            background-color:#EE9200
        }
        .fundo_td_laranja:hover{
            background-color:#EBB513
        }
    </style>

    <script>

    function abreRelatorioFinanceiro(vano, vsubacao){
        window.open('/financeiro/financeiro.php?modulo=relatorio/geral_teste&acao=R&painel=1&submetido=1&ano='+vano+'&escala=1&agrupador[0]=subacao&agrupador[1]=acacod&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=7&agrupadorColunas[3]=92&subacao[0]='+vsubacao+'&alterar_ano=0','Relatorio','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
    }

    $('.linkFinanceiro').live('click',function(){
        abreRelatorioFinanceiro(2012,$(this).attr('sigla'))
        return false;
    });

    $(function(){

        $(".tile").mousedown(function(){
            $(this).addClass("selecionado");
        });

        $(".tile").mouseup(function(){
            $(this).removeClass("selecionado");
        });

        $('.area').click(function(){

            $('#td_content_table').html('');

            id = this.id.split('_')[1];
            switch(id){
                    case 'pe':
                            $('.eixo').show();
                            $('#caminho_nivel1').html('Planejamento Estratégico');
                            $('#caminho_nivel2').html('');
                            $('#caminho_nivel3').html('');
                            break;
                    case 'ppa':
                            //$('.eixo').hide();
                            //$('#div_educacao_basica, #div_educacao_profissional, #div_educacao_superior').show();
                            $('.eixo').show();
                            $('#caminho_nivel1').html('Plano Plurianual');
                            $('#caminho_nivel2').html('');
                            $('#caminho_nivel3').html('');
                            break;
                    case 'pne':
                            //$('.eixo').hide();
                            //carregarMetasPne();
                            $('.eixo').show();
                            $('#caminho_nivel1').html('Plano Nacional de Educação');
                            $('#caminho_nivel2').html('');
                            $('#caminho_nivel3').html('');
                            break;
                    case 'ae':
                            $('.eixo').show();
                            $('#caminho_nivel1').html('Ações Estratégicas');
                            $('#caminho_nivel2').html('');
                            $('#caminho_nivel3').html('');
                            break;
                    case 'orc':
                            $('.eixo').show();
                            $('#caminho_nivel1').html('Orçamento');
                            $('#caminho_nivel2').html('');
                            $('#caminho_nivel3').html('');
                            break;
                    case 'pro':
                            $('.eixo').show();
                            $('#caminho_nivel1').html('Projetos');
                            $('#caminho_nivel2').html('');
                            $('#caminho_nivel3').html('');
                            break;
            }
            $('#div_roll').html('');

            $('.area').addClass('nao_selecionado').removeClass('marcado');
            $(this).removeClass('nao_selecionado').addClass('marcado');
            $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        });


        $('.eixo').click(function(){
            $('#td_content_table').html('');

            idTema = $(this).children().attr('id');
            idArea = $('.marcado').attr('id').split('_')[1];

            var classeCss;
            switch(idTema){
                case '1':
                    classeCss = 'azulClaro';
                    $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                    $('#caminho_nivel3').html('');
                    break;
                case '2':
                    classeCss = 'azulEscuro';
                    $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                    $('#caminho_nivel3').html('');
                    break;
                case '3':
                    classeCss = 'marromClaro';
                    $('#caminho_nivel2').html(' > Educação Superior');
                    $('#caminho_nivel3').html('');
                    break;
                case '4':
                    classeCss = 'verdeEscuro';
                    $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                    $('#caminho_nivel3').html('');
                    break;
                case '5':
                    classeCss = 'laranja';
                    $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                    $('#caminho_nivel3').html('');
                    break;
                case '6':
                    classeCss = 'roxo';
                    $('#caminho_nivel2').html(' > Suporte');
                    $('#caminho_nivel3').html('');
                    break;
            }

            $('.eixo').addClass('nao_selecionado').removeClass('marcado');
            $(this).removeClass('nao_selecionado').addClass('marcado');

            if(idTema>0){
                $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaRoll&temid='+idTema+'&area='+idArea+'&classe='+classeCss,
                    success	: function(e){
                            $('#div_roll').html(e);
                    }
                });
            }
        });

        $('.objetivos_metas').live('click', function(){
            $('#td_content_table').html('');
            arData = this.id.split('_');
            $('#objetivos_metas').val(arData[1]);
            $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
            $(this).removeClass('nao_selecionado').addClass('marcado');
            $('#caminho_nivel3').html(' > '+delimitador($(this).html(), 25));
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaTable&id='+this.id,
                    success	: function(e){
                            $('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
                            $('#td_content_table').html(e);
                            if(arData[0] == 'pne') carregaTablePNE(arData[1]);
                    }
            });
        });

        $('.desafios').live('click',function(){
            id = this.id;
            arData = this.id.split('_');
            // Carrega objetivos PPA
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaMetasPPAPE&id='+arData[1],
                    success	: function(e){
                            $('#div_objetivo_ppa').html(e);
                    }
            });
            // Carrega metas PNE
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaMetasPNE&id='+arData[1],
                    success	: function(e){
                            $('#div_metas_pne').html(e);
                    }
            });

            // Carrega metas e ações
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaMetasAcoesPE&id='+arData[1],
                    success	: function(e){
                            $('#div_acoes_metas').html(e);
                    }
            });


            // Carrega Orçamentos
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaOrcamentosPE&id='+arData[1],
                    success	: function(e){
                            $('#div_orcamentos').html(e);
                    }
            });

            // Carrega Projetos
            $.ajax({
                url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type	: 'post',
                data	: 'requisicao=carregaProjetos&id='+arData[1],
                success	: function(e){
                    $('#div_projetos').html(e);
                }
            });

            $('.desafios').addClass('nao_selecionado').removeClass('marcado');
            $(this).removeClass('nao_selecionado').addClass('marcado');
        });

        $('.metas, .iniciativas').live('click', function(){
            arIds = this.id.split('_');
            // Carrega Desafios PE
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaDesafiosPE&id='+arIds[1],
                    success	: function(e){
                            $('#div_desafios_pe').html(e);
                    }
            });

            // Carrega Objetivos Estratégicos PE
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaObjetivosEstrategicosPE&id='+arIds[1],
                    success	: function(e){
                            $('#div_objetivos_estrategicos_pe').html(e);
                    }
            });

            // Carrega Metas PNE e PPA
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaMetasPNEPPA&id='+arIds[1],
                    success	: function(e){
                            $('#div_metas_pne').html(e);
                    }
            });

            // Carrega Ações e Metas
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaAcoesPEPPA&id='+arIds[1],
                    success	: function(e){
                            $('#div_acoes_metas').html(e);
                    }
            });

            // Carrega Orçamentos
            $.ajax({
                    url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                    type	: 'post',
                    data	: 'requisicao=carregaOrcamentosPPA&id='+arIds[1],
                    success	: function(e){
                            $('#div_orcamentos').html(e);
                    }
            });

            // Carrega Projetos
            $.ajax({
                url		: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type	: 'post',
                data	: 'requisicao=carregaProjetosPPA&id='+arIds[1],
                success	: function(e){
                    $('#div_projetos_ppa').html(e);
                }
            });

            $('.metas').removeClass('nao_selecionado').removeClass('marcado');;
            $('.iniciativas').removeClass('nao_selecionado').removeClass('marcado');;
            $('.metas').addClass('nao_selecionado').removeClass('marcado');
            $('.iniciativas').addClass('nao_selecionado').removeClass('marcado');
            $(this).removeClass('nao_selecionado').addClass('marcado');
        });

        $('.clickMetasPNE').live('click',function(){
            param = this.id;
            idTema = param.split('_')[0];
            idMeta = param.split('_')[1];
            selecionaMetaPNE(idTema, idMeta);
        });

        $('.clickMetasPPA').live('click',function(){
            param = this.id;
            idTema = param.split('_')[0];
            idObjetivosMetas = param.split('_')[1];
            idMeta = param.split('_')[2];
            tipo = param.split('_')[3];
            selecionaMetaPPA(idTema, idObjetivosMetas, idMeta, tipo);
        });

        $('.clickDesafiosPE').live('click', function() {
            param = this.id;
            idTema = param.split('_')[0];
            idOpeid = param.split('_')[1];
            idDesid = param.split('_')[2];
            selecionaDesafioPE(idTema, idOpeid, idDesid);
        });
        $('.clickObjetivoEstrategico').live('click', function() {
            param = this.id;
            idTema = param.split('_')[0];
            idObeid = param.split('_')[1];
            selecionaObjetivoEstrategico(idTema, idObeid);
        });

        $('.clickAcoesEstrategicas').live('click',function(){
            param = this.id;
            idTema = param.split('_')[0];
            idAcao = param.split('_')[1];
            selecionaAcao(idTema, idAcao);
        });

        $('.clickOrcamentos').live('click',function(){
            param = this.id;
            idTema = param.split('_')[0];
            idOrc = param.split('_')[1];
            selecionaOrcamento(idTema, idOrc);
        });

        $('.clickProjetos').live('click',function(){
            param = this.id;
            idTema = param.split('_')[0];
            idProjeto = param.split('_')[1];
            selecionaProjeto(idTema, idProjeto);
        });

        $('.linkPopupIndicador').live('click', function() {
            var popUp = window.open(this.href, 'popIndicador', 'height=600,width=800,scrollbars=yes,top=50,left=200');
            popUp.focus();
            return false;
        });
        $('.linkPopupPainel').live('click', function() {
            var popUp = window.open(this.href, 'popPainel', 'height=600,width=800,scrollbars=yes,top=50,left=200');
            popUp.focus();
            return false;
        });
    });

    function carregarMetasPne(){
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregaRoll&area=pne',
            success: function(e) {
                $('#div_roll').html(e);
            }
        });
    }

    function carregaTablePNE(id){
        // carrega estrategias (div principal)
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregaEstrategiasPNE&id=' + id,
            success: function(e) {
                $('#td_estrategias_pne').html(e);
            }
        });

        // Carrega metas PPA
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregaMetaPPA&id=' + id,
            success: function(e) {
                $('#td_objetivos_ppa').html(e);
            }
        });

        // Carrega Desafios (planejamento estrategico)
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregarDesafiosPNE&id=' + id,
            success: function(e) {
                $('#td_planejamento_estrategico').html(e);
            }
        });
        // Carrega Objetivos Estratégicos
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregarObjetivosEstrategicosPNE&id=' + id,
            success: function(e) {
                $('#td_objetivo_estrategico_pne').html(e);
            }
        });

        //Carrega estrategias (açoes / metas)
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregaAcoesPDE&id=' + id,
            success: function(e) {
                $('#td_acoes_metas').html(e);
            }
        });

        // orcamentos
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregaOrcamentos&id=' + id,
            success: function(e) {
                $('#td_orcamentos').html(e);
            }
        });

        // projetos
        $.ajax({
            url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
            type: 'post',
            data: 'requisicao=carregaProjetosPNE&id=' + id,
            success: function(e) {
                $('#td_projetos_pne').html(e);
            }
        });
    }

    function delimitador(texto, qtd){
        qtd = qtd ? qtd : 150;
        total = texto.lenght;
        if (total == qtd) {
            return texto;
        } else {
            return texto.substr(0, qtd) + '...';
        }
    }

    function buscar(busca) {
        window.open('/painel/painel.php?modulo=principal/painel_controle&acao=A&buscacockpit=' + busca, 'Observações', 'scrollbars=yes,height=800,width=1500,status=no,toolbar=no,menubar=no,location=no');
    }

    function exibeBuscaEnter(event) {
        if (event.keyCode == 13) {
            buscar(document.getElementById('busca').value);
        }
    }

    function selecionaMetaPNE(idTema, idMeta){
        //limpa tela
        $('#td_content_table').html('');
        $('#div_roll').html('');
        $('#caminho_nivel1').html('');
        $('#caminho_nivel2').html('');
        $('#caminho_nivel3').html('');

        //area
        $('.area').removeClass('nao_selecionado').removeClass('marcado');
        $('.area').addClass('nao_selecionado').removeClass('marcado');
        $('#div_pne').removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel1').html('Plano Nacional de Educação');

        //eixo
        $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        $('.eixo').addClass('nao_selecionado').removeClass('marcado');

        idArea = $('.marcado').attr('id').split('_')[1];
        var classeCss;

        switch(idTema){
            case '1':
                classeCss = 'azulClaro';
                $('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                $('#caminho_nivel3').html('');
                break;
            case '2':
                classeCss = 'azulEscuro';
                $('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                $('#caminho_nivel3').html('');
                break;
            case '3':
                classeCss = 'marromClaro';
                $('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Superior');
                $('#caminho_nivel3').html('');
                break;
            case '4':
                classeCss = 'verdeEscuro';
                $('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                $('#caminho_nivel3').html('');
                break;
            case '5':
                classeCss = 'laranja';
                $('#div_formacao').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                $('#caminho_nivel3').html('');
                break;
            case '6':
                classeCss = 'roxo';
                $('#div_suporte').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Suporte');
                $('#caminho_nivel3').html('');
                break;
        }

        if (idTema > 0) {
            $.ajax({
                url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type: 'post',
                data: 'requisicao=carregaRoll&temid=' + idTema + '&area=' + idArea + '&classe=' + classeCss,
                success: function(e) {
                    $('#div_roll').html(e);
                    $('#objetivos_metas').val(idMeta);
                    $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
                    $('#pne_' + idMeta).removeClass('nao_selecionado').addClass('marcado');
                    $('#caminho_nivel3').html(' > ' + delimitador($('#pne_' + idMeta).html(), 25));
                    $('#pne_' + idMeta).trigger('click');
                }
            });
        }
    }

    function selecionaMetaPPA(idTema, idObjetivosMetas, idMeta, tipo){
        //limpa tela
        $('#td_content_table').html('');
        $('#div_roll').html('');
        $('#caminho_nivel1').html('');
        $('#caminho_nivel2').html('');
        $('#caminho_nivel3').html('');

        //area
        $('.area').removeClass('nao_selecionado').removeClass('marcado');
        $('.area').addClass('nao_selecionado').removeClass('marcado');
        $('#div_ppa').removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel1').html('Plano Plurianual');

        //eixo
        $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        $('.eixo').addClass('nao_selecionado').removeClass('marcado');

        idArea = $('.marcado').attr('id').split('_')[1];
        var classeCss;

        switch(idTema){
            case '1':
                classeCss = 'azulClaro';
                $('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                $('#caminho_nivel3').html('');
                break;
            case '2':
                classeCss = 'azulEscuro';
                $('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                $('#caminho_nivel3').html('');
                break;
            case '3':
                classeCss = 'marromClaro';
                $('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Superior');
                $('#caminho_nivel3').html('');
                break;
            case '4':
                classeCss = 'verdeEscuro';
                $('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                $('#caminho_nivel3').html('');
                break;
            case '5':
                classeCss = 'laranja';
                $('#div_formacao').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                $('#caminho_nivel3').html('');
                break;
            case '6':
                classeCss = 'roxo';
                $('#div_suporte').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Suporte');
                $('#caminho_nivel3').html('');
                break;
        }

        if (idTema > 0) {
            $.ajax({
                url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type: 'post',
                data: 'requisicao=carregaRoll&temid=' + idTema + '&area=' + idArea + '&classe=' + classeCss,
                success: function(e) {
                    $('#div_roll').html(e);
                    $('#objetivos_metas').val(idObjetivosMetas);
                    $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
                    $('#ppa_' + idObjetivosMetas).removeClass('nao_selecionado').addClass('marcado');
                    $('#caminho_nivel3').html(' > ' + delimitador($('#ppa_' + idObjetivosMetas).html(), 25));
                    $.ajax({
                        url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                        type: 'post',
                        data: 'requisicao=carregaTable&id=ppa_' + idObjetivosMetas,
                        success: function(e) {
                            $('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
                            $('#td_content_table').html(e);
                            if (tipo == 'M')
                                $('#metas_' + idMeta).trigger('click');
                            else
                                $('#iniciativas_' + idMeta).trigger('click');
                        }
                    });
                }
            });
        }
    }

    function selecionaDesafioPE(idTema, idOpeid, idDesid){
        //limpa tela
        $('#td_content_table').html('');
        $('#div_roll').html('');
        $('#caminho_nivel1').html('');
        $('#caminho_nivel2').html('');
        $('#caminho_nivel3').html('');

        //area
        $('.area').removeClass('nao_selecionado').removeClass('marcado');
        $('.area').addClass('nao_selecionado').removeClass('marcado');
        $('#div_pe').removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel1').html('Planejamento Estratégico');

        //eixo
        $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        $('.eixo').addClass('nao_selecionado').removeClass('marcado');

        idArea = $('.marcado').attr('id').split('_')[1];

        var classeCss;

        switch(idTema){
            case '1':
                classeCss = 'azulClaro';
                $('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                $('#caminho_nivel3').html('');
                break;
            case '2':
                classeCss = 'azulEscuro';
                $('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                $('#caminho_nivel3').html('');
                break;
            case '3':
                classeCss = 'marromClaro';
                $('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Superior');
                $('#caminho_nivel3').html('');
                break;
            case '4':
                classeCss = 'verdeEscuro';
                $('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                $('#caminho_nivel3').html('');
                break;
            case '5':
                classeCss = 'laranja';
                $('#div_formacao').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                $('#caminho_nivel3').html('');
                break;
            case '6':
                classeCss = 'roxo';
                $('#div_suporte').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Suporte');
                $('#caminho_nivel3').html('');
                break;
        }

        if (idTema > 0) {
            $.ajax({
                url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type: 'post',
                data: 'requisicao=carregaRoll&temid=' + idTema + '&area=' + idArea + '&classe=' + classeCss,
                success: function(e) {
                    $('#div_roll').html(e);
                    $('#objetivos_metas').val(idOpeid);
                    $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
                    $('#pe_' + idOpeid).removeClass('nao_selecionado').addClass('marcado');
                    $('#caminho_nivel3').html(' > ' + delimitador($('#pe_' + idOpeid).html(), 25));
                    $.ajax({
                        url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                        type: 'post',
                        data: 'requisicao=carregaTable&id=pe_' + idOpeid,
                        success: function(e) {
                            $('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
                            $('#td_content_table').html(e);
                            $('.desafios').addClass('nao_selecionado').removeClass('marcado');
                            $('#des_' + idDesid).removeClass('nao_selecionado').addClass('marcado');
                            $('#caminho_nivel3').html(' > ' + delimitador($('#des_' + idDesid).html(), 25));
                            $('#des_' + idDesid).trigger('click');

                        }
                    });
                }
            });
        }
    }

    function selecionaObjetivoEstrategico(idTema, idObeid){
        //limpa tela
        $('#td_content_table').html('');
        $('#div_roll').html('');
        $('#caminho_nivel1').html('');
        $('#caminho_nivel2').html('');
        $('#caminho_nivel3').html('');

        //area
        $('.area').removeClass('nao_selecionado').removeClass('marcado');
        $('.area').addClass('nao_selecionado').removeClass('marcado');
        $('#div_obj').removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel1').html('Objetivos Estratégicos');

        //eixo
        $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        $('.eixo').addClass('nao_selecionado').removeClass('marcado');

        idArea = $('.marcado').attr('id').split('_')[1];
        var classeCss;

        switch(idTema){
            case '1':
                classeCss = 'azulClaro';
                $('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                $('#caminho_nivel3').html('');
                break;
            case '2':
                classeCss = 'azulEscuro';
                $('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                $('#caminho_nivel3').html('');
                break;
            case '3':
                classeCss = 'marromClaro';
                $('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Superior');
                $('#caminho_nivel3').html('');
                break;
            case '4':
                classeCss = 'verdeEscuro';
                $('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                $('#caminho_nivel3').html('');
                break;
            case '5':
                classeCss = 'laranja';
                $('#div_formacao').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                $('#caminho_nivel3').html('');
                break;
            case '6':
                classeCss = 'roxo';
                $('#div_suporte').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Suporte');
                $('#caminho_nivel3').html('');
                break;
        }

        if (idTema > 0) {
            $.ajax({
                url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type: 'post',
                data: 'requisicao=carregaRoll&temid=' + idTema + '&area=' + idArea + '&classe=' + classeCss,
                success: function(e) {
                    $('#div_roll').html(e);
                    $('#objetivos_metas').val(idObeid);
                    $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
                    $('#obj_' + idObeid).removeClass('nao_selecionado').addClass('marcado');
                    $('#caminho_nivel3').html(' > ' + delimitador($('#obj_' + idObeid).html(), 25));
                    $('#obj_' + idObeid).trigger('click');
                }
            });
        }
    }

    function selecionaAcao(idTema, idAcao){
        //limpa tela
        $('#td_content_table').html('');
        $('#div_roll').html('');
        $('#caminho_nivel1').html('');
        $('#caminho_nivel2').html('');
        $('#caminho_nivel3').html('');

        //area
        $('.area').removeClass('nao_selecionado').removeClass('marcado');
        $('.area').addClass('nao_selecionado').removeClass('marcado');
        $('#div_ae').removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel1').html('Ações Estratégicas');

        //eixo
        $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        $('.eixo').addClass('nao_selecionado').removeClass('marcado');

        idArea = $('.marcado').attr('id').split('_')[1];
        var classeCss;

        switch(idTema){
            case '1':
                classeCss = 'azulClaro';
                $('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                $('#caminho_nivel3').html('');
                break;
            case '2':
                classeCss = 'azulEscuro';
                $('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                $('#caminho_nivel3').html('');
                break;
            case '3':
                classeCss = 'marromClaro';
                $('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Superior');
                $('#caminho_nivel3').html('');
                break;
            case '4':
                classeCss = 'verdeEscuro';
                $('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                $('#caminho_nivel3').html('');
                break;
            case '5':
                classeCss = 'laranja';
                $('#div_formacao').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                $('#caminho_nivel3').html('');
                break;
            case '6':
                classeCss = 'roxo';
                $('#div_suporte').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Suporte');
                $('#caminho_nivel3').html('');
                break;
        }

        if (idTema > 0) {
            $.ajax({
                url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type: 'post',
                data: 'requisicao=carregaRoll&temid=' + idTema + '&area=' + idArea + '&classe=' + classeCss,
                success: function(e) {
                    $('#div_roll').html(e);
                    $('#objetivos_metas').val(idAcao);
                    $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
                    $('#ae_' + idAcao).removeClass('nao_selecionado').addClass('marcado');
                    $('#caminho_nivel3').html(' > ' + delimitador($('#ae_' + idAcao).html(), 25));
                    $('#ae_' + idAcao).trigger('click');
                }
            });
        }
    }

    function selecionaOrcamento(idTema, idOrc){
        //limpa tela
        $('#td_content_table').html('');
        $('#div_roll').html('');
        $('#caminho_nivel1').html('');
        $('#caminho_nivel2').html('');
        $('#caminho_nivel3').html('');

        //area
        $('.area').removeClass('nao_selecionado').removeClass('marcado');
        $('.area').addClass('nao_selecionado').removeClass('marcado');
        $('#div_orc').removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel1').html('Orçamento');

        //eixo
        $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        $('.eixo').addClass('nao_selecionado').removeClass('marcado');

        idArea = $('.marcado').attr('id').split('_')[1];
        var classeCss;

        switch(idTema){
            case '1':
                classeCss = 'azulClaro';
                $('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                $('#caminho_nivel3').html('');
                break;
            case '2':
                classeCss = 'azulEscuro';
                $('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                $('#caminho_nivel3').html('');
                break;
            case '3':
                classeCss = 'marromClaro';
                $('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Superior');
                $('#caminho_nivel3').html('');
                break;
            case '4':
                classeCss = 'verdeEscuro';
                $('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                $('#caminho_nivel3').html('');
                break;
            case '5':
                classeCss = 'laranja';
                $('#div_formacao').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                $('#caminho_nivel3').html('');
                break;
            case '6':
                classeCss = 'roxo';
                $('#div_suporte').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Suporte');
                $('#caminho_nivel3').html('');
                break;
        }

        if (idTema > 0) {
            $.ajax({
                url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type: 'post',
                data: 'requisicao=carregaRoll&temid=' + idTema + '&area=' + idArea + '&classe=' + classeCss,
                success: function(e) {
                    $('#div_roll').html(e);
                    $('#objetivos_metas').val(idOrc);
                    $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
                    $('#orc_' + idOrc).removeClass('nao_selecionado').addClass('marcado');
                    $('#caminho_nivel3').html(' > ' + delimitador($('#orc_' + idOrc).html(), 25));
                    $('#orc_' + idOrc).trigger('click');
                }
            });
        }
    }

    function selecionaProjeto(idTema, idProjeto){
        //limpa tela
        $('#td_content_table').html('');
        $('#div_roll').html('');
        $('#caminho_nivel1').html('');
        $('#caminho_nivel2').html('');
        $('#caminho_nivel3').html('');

        //area
        $('.area').removeClass('nao_selecionado').removeClass('marcado');
        $('.area').addClass('nao_selecionado').removeClass('marcado');
        $('#div_pro').removeClass('nao_selecionado').addClass('marcado');
        $('#caminho_nivel1').html('Projetos');

        //eixo
        $('.eixo').removeClass('nao_selecionado').removeClass('marcado');
        $('.eixo').addClass('nao_selecionado').removeClass('marcado');

        idArea = $('.marcado').attr('id').split('_')[1];
        var classeCss;

        switch(idTema){
            case '1':
                classeCss = 'azulClaro';
                $('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Acesso e Qualidade da Educação Básica');
                $('#caminho_nivel3').html('');
                break;
            case '2':
                classeCss = 'azulEscuro';
                $('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
                $('#caminho_nivel3').html('');
                break;
            case '3':
                classeCss = 'marromClaro';
                $('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Educação Superior');
                $('#caminho_nivel3').html('');
                break;
            case '4':
                classeCss = 'verdeEscuro';
                $('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Planejamento e Gestão das Políticas Educacionais');
                $('#caminho_nivel3').html('');
                break;
            case '5':
                classeCss = 'laranja';
                $('#div_formacao').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Formação e Valorização de Professores e Profissionais da Educação');
                $('#caminho_nivel3').html('');
                break;
            case '6':
                classeCss = 'roxo';
                $('#div_suporte').removeClass('nao_selecionado').addClass('marcado');
                $('#caminho_nivel2').html(' > Suporte');
                $('#caminho_nivel3').html('');
                break;
        }

        if (idTema > 0) {
            $.ajax({
                url: 'estrategico.php?modulo=principal/alinhamento_estrategico&acao=A',
                type: 'post',
                data: 'requisicao=carregaRoll&temid=' + idTema + '&area=' + idArea + '&classe=' + classeCss,
                success: function(e) {
                    $('#div_roll').html(e);
                    $('#objetivos_metas').val(idProjeto);
                    $('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
                    $('#pro_' + idObeid).removeClass('nao_selecionado').addClass('marcado');
                    $('#caminho_nivel3').html(' > ' + delimitador($('#pro_' + idProjeto).html(), 25));
                    $('#pro_' + idProjeto).trigger('click');
                }
            });
        }
    }

    function changeSystem(system)
    {
        location.href = "../muda_sistema.php?sisid=" + system;
    }
    </script>

    </head>
        <body style="background: none;">
            <input type="hidden" name="objetivos_metas" id="objetivos_metas" value="" />

            <table width="100%" border="0">
                <tr>
                    <td align="left" width="40%" style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A&completo=1';">
                        <img src="./cockpit/images/logo_simec.png" style="vertical-align:middle;" alt="" title="" />
                    </td>
                    <td align=center width="50%">
                        <?php $sistemas = carregarSistemas(); ?>
                        <div style="float: left;">
                            <div style="margin-top: 10px;" class="cabecalho">
                                <select data-placeholder="Escolha um módulo do sistema..." id="cabecalho" class="chosen-select" style="width:200px;" tabindex="2" onchange="javascipt:changeSystem(this.value);">
                                    <?php if ($sistemas && is_array($sistemas)) : ?>
                                        <?php foreach ($sistemas as $sistema): ?>
                                            <option <?php if($_SESSION['sisid'] == $sistema['sisid']) echo 'selected="true"' ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                        <?php endforeach; ?>
                                    <?php endif; ?>
                                </select>
                            </div>
                        </div>
                    </td>
                    <td align=center width="5%">
                        <a style="vertical-align:middle;" href="wap/"><img  style="vertical-align:middle;" src="cockpit/images/mobileicon2.png"><br><font color="white">Mobile</font></a>
                    </td>
                    <td align="right" width="5%" style="cursor:pointer;" >
                        <img src="./cockpit/images/bt_atualizar.png" onclick="window.location='estrategico.php?modulo=principal/alinhamento_estrategico&acao=A';" style="vertical-align:middle;" alt="" title="" />
                    </td>
                </tr>
            </table>

            <br/>

            <table width="100%">
                <tr>
                    <td class="fundo_td_laranja" colspan="4">
                        <div style="text-align:center;" id='divbusca'  >
                            <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
                            <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura"  onfocus="this.select();" onblur="if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
                            <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
                            <p style="margin-top:0;">
                                Pesquise aqui as iniciativas do Ministério da Educação por: Estados, Municípios, Universidades, Escolas, Regiões, Territórios, Ações,
                                Programas, Indicadores, Secretarias, Autarquias, etc.
                            </p>
                        </div>
                    </td>
                </tr>
            </table>

            <br/>

            <table  width="100%" cellpadding="0" cellspacing="10" border="0">
                <tr>
                    <td align="right" valign="bottom" colspan="2">
                        <span id="caminho_nivel1" style="font-size:14px;"></span>
                        <span id="caminho_nivel2" style="font-size:14px;"></span>
                        <span id="caminho_nivel3" style="font-size:14px;"></span>
                    </td>
                </tr>
                <tr>
                    <td align=center colspan= 2>
                        <table>
                            <tr>
                                <td id="td_acoes_estrategicas">
                                    <div id="div-acoes-estrategicas" style="width:160px;height:60px;background:gray;display:table-cell;vertical-align:middle;font-family:  Verdana;font-size:12px;cursor:pointer" onclick="$('#divalinhamento').hide(); $('#divplanejamento').show();" >
                                        <img style="float:left;" src="../imagens/icones/icons/mapas.png" > <br><b>Ações Estratégicas
                                    </div>
                                </td>
                                <td id="td_alinhamento_estrategico">
                                    <div  id="div-alinhamento-estrategico" style="width:160px;height:60px;background:navy;display:table-cell;vertical-align:middle;font-family: Verdana;font-size:12px;cursor:pointer" onclick="$('#divalinhamento').show(); $('#divplanejamento').hide();" >
                                        <img style="float:left;" src="../imagens/icones/icons/casas.png" > <br><b>Alinhamento Estratégico
                                    </div>
                                </td>
                                <!--<td id="td_temas">
                                    <a href="estrategico.php?modulo=principal/temas&acao=A">
                                        <div  id="div-temas" style="width:160px;height:60px;background:navy;display:table-cell;vertical-align:middle;font-family: Verdana;font-size:12px;cursor:pointer" >
                                            <img style="float:left;" src="../imagens/icones/icons/casas.png" > <br><b>Temas
                                        </div>
                                    </a>
                                </td>-->
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>

            <div width = 90% id="divplanejamento">
                <br>
                <?php
                    include 'include_painel_cockpit.inc';
                ?>
            </div>

            <div id = 'divalinhamento'>
                <table width="98%" border="0">
                    <tr>
                        <td width="45%" align="left" valign="top">
                            <div class="pagina" >

                                <div class="linha">
                                    <div class="tile0 tile2 tile3 verde area" style="height:70px" id="div_pne">
                                        <span class="titulo">
                                            <img src="../imagens/icones/icons/alvo.png" align="top" style="float:left; width: 40px;height: 40px">
                                                Plano Nacional de Educação</span>
                                    </div>
                                    <div class="tile0 tile2 tile3 azul area" style="height:70px" id="div_ppa">
                                        <span class="titulo">
                                            <img src="../imagens/icones/icons/configs.png" align="top" style="float:left; width: 40px;height: 40px;">
                                                Plano Plurianual</span>
                                    </div>
                                    <!--<div class="tile0 tile2 tile3 amarelo area" style="height:70px" id="div_pe">
                                        <span class="titulo">
                                            <img src="../imagens/icones/icons/doc.png" align="top" style="float:left; width: 40px;height: 40px;">
                                                Objetivos Estratégicos e Desafios</span>
                                    </div>-->
                                    <div class="tile0 tile2 tile3 amarelo area" style="height:70px" id="div_obj">
                                        <span class="titulo">
                                            <img src="../imagens/icones/icons/doc.png" align="top" style="float:left; width: 40px;height: 40px;">
                                                Objetivos Estratégicos</span>
                                    </div>
                                </div>

                                <div class="linha">
                                    <div class="tile0 tile2 tile3 laranja area" style="height:70px" id="div_ae">
                                        <span class="titulo">
                                            <img src="../imagens/icones/icons/indicador.png" align="top" style="float:left; width: 40px;height: 40px">
                                                Ações Estratégicas</span>
                                    </div>
                                    <div class="tile0 tile2 tile3 roxo area" style="height:70px" id="div_orc">
                                        <span class="titulo">
                                            <img src="../imagens/icones/icons/financeiro.png" align="top" style="float:left; width: 40px;height: 40px;">
                                                Orçamento</span>
                                    </div>
                                    <div class="tile0 tile2 tile3 marromClaro area" style="height:70px" id="div_pro">
                                        <span class="titulo">
                                            <img src="../imagens/icones/icons/doc.png" align="top" style="float:left; width: 40px;height: 40px;">
                                                Projetos</span>
                                    </div>
                                </div>

                                <div class="linha">
                                    <div class="tile1 azulClaro eixo" id="div_educacao_basica" style="display:none;">
                                        <span class="SubTitulo" id="1">Acesso e Qualidade da Educação Básica</span><br/>
                                    </div>
                                    <div class="tile1 azulEscuro eixo" id="div_educacao_profissional" style="display:none;">
                                        <span class="SubTitulo" id="2">Educação Profissional e Tecnológica</span><br/>
                                    </div>
                                    <div class="tile1 marromClaro eixo" id="div_educacao_superior" style="display:none;">
                                        <span class="SubTitulo" id="3">Educação Superior</span><br/>
                                    </div>
                                </div>
                                <div class="linha">
                                    <div class="tile1 verdeEscuro eixo" id="div_planejamento" style="display:none;">
                                        <span class="SubTitulo" id="4">Planejamento e Gestão das Políticas Educacionais</span><br/>
                                    </div>
                                    <div class="tile1 laranja eixo" id="div_formacao" style="display:none;">
                                        <span class="SubTitulo" id="5">Formação e Valorização de Professores e Profissionais da Educação</span><br/>
                                    </div>
                                    <div class="tile1 roxo eixo" id="div_suporte" style="display:none;">
                                        <span class="SubTitulo" id="6">Suporte</span><br/>
                                    </div>
                                </div>
                            </div>

                            <div  id="div_roll"></div>

                        </td>
                        <td width="55%" align="center" valign="top" id="td_content_table" style="background: none;">
                            &nbsp;
                        </td>
                    </tr>
                </table>
            </div>
        </body>
    </html>

<?php

function montaGridPE()
{
	global $db;
	$arIds = explode('_', $_REQUEST['id']);
	$sql = "select desid, desnome from pde.ae_desafio where opeid = ".$arIds[1]. "order by desnome";
	$rs = $db->carregar($sql);
	?>
	<?php if($rs): ?>
		<div class="pagina" style="width:100%">
			<table width="100%">
				<tr>
					<td bgcolor="#4682B4" width="33%"><span class="tituloInterno">Desafios</span></td>
				</tr>
				<tr>
					<td align="left" valign="top" style="padding-right:4px;">
						<div id="div_roll_pagina">
						<?php foreach($rs as $data): ?>
							<div class="tile1  tile4 azulClaro desafios" id="des_<?php echo $data['desid']?>">
							    <span class="textoDescricao"><?php echo $data['desnome']; ?></span><br/>
							</div>
						<?php endforeach; ?>
						</div>
					</td>
				</tr>
			</table>
			<table width="100%">
				<tr>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas / Iniciativas PPA</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas PNE</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Ações Estratégicas</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Orçamentos</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Projetos</span></td>
				</tr>
				<tr>
					<td id="div_objetivo_ppa" align="left" valign="top" style="padding-right:4px;"></td>
					<td id="div_metas_pne" align="left" valign="top" style="padding-right:4px;"></td>
					<td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;"></td>
					<td id="div_orcamentos" align="left" valign="top" style="padding-right:4px;"></td>
					<td id="div_projetos" align="left" valign="top" style="padding-right:4px;"></td>
				</tr>
			</table>
		</div>
	<?php endif; ?>
	<?php
}

function montaGridOBJ()
{
    global $db;
    $arIds = explode('_', $_REQUEST['id']);
    ?>
    <div class="pagina" style="width:100%">
        <table width="100%" cellpadding="4">
            <tr>
                <td bgcolor="#4682B4" width="100%"><span class="titulo">Iniciativas PEI</span></td>
            </tr>
            <tr>
                <td align="left" valign="top">
                    <div id="div_roll_pagina">
                        <?php
                        $sql = "SELECT ini.ininome
                                FROM pde.ae_iniciativa ini
                                INNER JOIN pde.ae_objetivoestrategicoxiniciativa ins ON ins.iniid = ini.iniid
                                WHERE ins.obeid = ".$arIds[1]. "
                                ORDER BY ini.ininome";
                        $rs = $db->carregar($sql);
                        if ($rs) {
                            foreach($rs as $data){ ?>
                                <div class="tile1 tile4 azulClaro iniciativasPEI" style="cursor: default">
                                    <span class="textoDescricao"><?php echo $data['ininome']; ?></span><br/>
                                </div>
                            <?php }
                        } ?>
                    </div>
                </td>
            </tr>
        </table>
        <table width="100%">
            <tr>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas PNE</span></td>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas / Iniciativas PPA</span></td>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Ações Estratégicas</span></td>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Orçamentos</span></td>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Projetos</span></td>
            </tr>
            <tr>
                <td id="div_metas_pne" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select mpneid from pde.ae_objetivoestrategicoxmetapne where obeid =".$arIds[1];
                    $dados = $db->carregarColuna($sql,'mpneid');
                    $metas = implode(',',$dados);
                    if ($metas)
                    {
                        $sql = "select
										m.temid,
										m.mpneid as codigo,
										coalesce(ind.link,'') || m.mpnenome as nome
									from pde.ae_metapne m
									left join
										(
										select
											'<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
											mpneid
										from pde.ae_metapnexindicador group by mpneid
										) as ind ON ind.mpneid = m.mpneid
									where m.mpneid in ($metas)
									and m.mpneid <> 99999999
									order by m.mpneordem";
                        $rs = $db->carregar($sql);

                        if ($rs)
                        {
                            foreach($rs as $data)
                            {
                                echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                            }
                        }
                    }
                    ?>
                </td>
                <td id="div_objetivo_ppa" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select mppaid from pde.ae_objetivoestrategicoxmetappa where obeid =".$arIds[1];
                    $dados = $db->carregarColuna($sql,'mppaid');
                    $metas = implode(',',$dados);

                    if ($metas)
                    {
                        $sql = "select mppaid as codigo, mppanome as nome, temid, o.objid, mppatipo from pde.ae_metappa m
							inner join pde.ae_objetivoppa o on m.objid = o.objid where mppaid in ($metas) order by mppatipo DESC, mppanome";
                        $rs = $db->carregar($sql);

                        if ($rs)
                        {
                            foreach($rs as $data)
                            {
                                echo '<div id="'.$data['temid'].'_'.$data['objid'].'_'.$data['codigo'].'_'.$data['mppatipo'].'" class="tile1 tile4 azul clickMetasPPA">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                            }
                        }
                    }
                    ?>
                </td>
                <td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select acaid from pde.ae_objetivoestrategicoxacao where obeid =".$arIds[1];
                    $dados = $db->carregarColuna($sql, 'acaid');
                    $acoes = implode(',', $dados);

                    if ($acoes) {
                        $sql = "
                                select aca.temid, aca.acaid as codigo, aca.acadsc as nome, aca.acadetalhe, aca.indidprincipal, coc.coclink
                                from painel.acao aca
                                left join pde.cockpit coc ON coc.cocid = aca.cocid
                                where aca.acaid in ($acoes)
                                order by aca.acadsc
                            ";
                                        $rs = $db->carregar($sql);
                                        if ($rs) {
                                            foreach ($rs as $data) {
                                                echo '
                                        <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['codigo'] . '" >
                                            ' . ($data['coclink'] ? '<a href="' . $data['coclink'] . '" target="_blank" class="linkPopupPainel" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '') . '
                                            ' . ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid=' . $data['indidprincipal'] . '" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '') . '
                                            <a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid=' . $data['codigo'] . '" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;
                                            <span class="textoDescricao" title="' . $data['acadetalhe'] . '" alt="' . $data['acadetalhe'] . '">' . $data['nome'] . '</span><br/>
                                        </div>
                                    ';
                                            }
                                        }
                                    }
                    ?>
                </td>
                <td id="div_orcamentos" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select acaid from pde.ae_objetivoestrategicoxacao where obeid =".$arIds[1];
                    $dados = $db->carregarColuna($sql, 'acaid');
                    $orcamentos = implode(',', $dados);

                    if ($orcamentos) {
                        $sql = "SELECT vacid, vaetituloorcamentario AS nome
                                FROM planacomorc.vinculacaoacaoestrategica
                                WHERE acaid in ($orcamentos)
                                ORDER BY nome";
                        $rs = $db->carregar($sql);
                        if ($rs) {
                            foreach ($rs as $data) {
                                echo '
                                    <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');">
                                        <span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                                    </div>
                                ';
                            }
                        }
                    }
                    ?>
                </td>
                <td id="div_projetos" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select solid from pto.objetivosolucao where obeid =".$arIds[1];
                    $dados = $db->carregarColuna($sql,'solid');
                    $solucoes = implode(',',$dados);

                    if ($solucoes)
                    {
                        $sql = "SELECT DISTINCT tes.temid, sol.solid AS codigo, COALESCE(sol.solapelido,sol.soldsc) AS nome, sol.solordem
                                    FROM pto.solucao sol
                                    INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                                    WHERE sol.solstatus = 'A'
                                    AND sol.solid IN ($solucoes)
                                    ORDER BY sol.solordem";
                        $rs = $db->carregar($sql);

                        if ($rs)
                        {
                            foreach($rs as $data)
                            {
                                echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 marromClaro clickProjetos">
                                            <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
						  		 		    <span class="textoDescricao">'.$data['nome'].'</span><br/>
						  		 		  </div>';
                            }
                        }
                    }
                    ?>
                </td>
            </tr>
        </table>
    </div>
<?php
}

function montaGridPPA()
{
	global $db;
	$arIds = explode('_', $_REQUEST['id']);
	$sql = "select mppaid, mppanome, mppatipo from pde.ae_metappa where objid = {$arIds[1]}";
	$rs = $db->carregar($sql);
	?>
	<?php if($rs): ?>
		<div class="pagina" style="width:100%">

			<table width="100%" cellpadding="4">
				<tr>
					<td bgcolor="#4682B4" width="50%"><span class="titulo">Metas PPA</span></td>
					<td bgcolor="#4682B4" width="50%"><span class="titulo">Iniciativas PPA</span></td>
				</tr>
				<tr>
					<td align="left" valign="top">
						<div id="div_roll_pagina">
						<?php foreach($rs as $data): ?>
							<?php if($data['mppatipo'] == 'M'): ?>
								<div class="tile1 tile4 azulClaro metas" id="metas_<?php echo($data['mppaid'])?>">
								    <span class="textoDescricao"><?php echo $data['mppanome']; ?></span><br/>
								</div>
							<?php endif; ?>
						<?php endforeach; ?>
						</div>
					</td>
					<td align="left" valign="top">
						<div id="div_roll_pagina">
						<?php foreach($rs as $data): ?>
							<?php if($data['mppatipo'] == 'I'): ?>
								<div class="tile1 tile4 azulClaro iniciativas" id="iniciativas_<?php echo($data['mppaid'])?>">
								    <span class="textoDescricao"><?php echo $data['mppanome']; ?></span><br/>
								</div>
							<?php endif; ?>
						<?php endforeach; ?>
						</div>
					</td>
				</tr>
			</table>

			<table width="100%" cellpadding="4" style="margin-left: 5px;">
				<tr>
					<td bgcolor="#4682B4" width="20%"><span class="titulo">Metas PNE</span></td>
                    <!--<td bgcolor="#4682B4" width="20%"><span class="titulo">Objetivos Estratégicos e Desafios</span></td>-->
                    <td bgcolor="#4682B4" width="20%"><span class="titulo">Objetivos Estratégicos</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="titulo">Ações Estratégicas</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="titulo">Orçamentos</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="titulo">Projetos</span></td>
				</tr>
				<tr>
					<td id="div_metas_pne" align="left" valign="top"></td>
                    <!--<td id="div_desafios_pe" align="left" valign="top"></td>-->
                    <td id="div_objetivos_estrategicos_pe" align="left" valign="top"></td>
					<td id="div_acoes_metas" align="left" valign="top"></td>
					<td id="div_orcamentos" align="left" valign="top"></td>
					<td id="div_projetos_ppa" align="left" valign="top"></td><!-- NÃO VINCULADO>
				</tr>
			</table>
		</div>
	<?php endif; ?>
	<?php
}


function montaGridAE()
{
	global $db;
	$arIds = explode('_', $_REQUEST['id']);
	$sql = "select aca.acaid, aca.acadetalhe, aca.indidprincipal, coc.coclink
			from painel.acao aca
			left join pde.cockpit coc ON coc.cocid = aca.cocid
			where aca.acaid = {$arIds[1]}";
	$rs = $db->carregar($sql);
	?>
	<div class="pagina" style="width:100%">
		<table width="100%" cellpadding="3">
			<tr>
				<td  bgcolor="#4682B4"><span class="titulo">Detalhe da Ação</span></td>
			</tr>
			<tr>
				<td colspan="3">
					<div id="div_roll_pagina">
						<div class="tile1  tile4 azulClaro acoes" style="cursor: default">
						<?
						if ($rs)
						{
						echo($rs[0]['coclink'] ? '<a href="'.$rs[0]['coclink'].'" class="linkPopupPainel" target="_blank" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>':'');
						echo($rs[0]['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid='.$rs[0]['indidprincipal'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;':'');
						echo('<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid='.$rs[0]['acaid'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;'.$rs[0]['acadetalhe']);
						}
						?>
						</div>
					</div>
				</td>
			</tr>
			<tr>
			<td>
			<table width="100%">
				<tr>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas PNE</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas / Iniciativas PPA</span></td>
					<!--<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos e Desafios</span></td>-->
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Orçamentos</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Projetos</span></td>
				</tr>
				<tr>
					<td id="div_metas_pne" align="left" valign="top" style="padding-right:4px;">
					<?php
						$sql = "select mpneid from pde.ae_acaoxmetapne where acaid =".$arIds[1];
						$dados = $db->carregarColuna($sql,'mpneid');
						$metas = implode(',',$dados);

						if ($metas)
						{
							$sql = "select 
										m.temid, 
										m.mpneid as codigo, 
										coalesce(ind.link,'') || m.mpnenome as nome 
									from pde.ae_metapne m
									left join
										(
										select
											'<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
											mpneid
										from pde.ae_metapnexindicador group by mpneid
										) as ind ON ind.mpneid = m.mpneid
									where m.mpneid in ($metas)
									and m.mpneid <> 99999999
									order by m.mpneordem";
							$rs = $db->carregar($sql);

							if ($rs)
							{
								foreach($rs as $data)
								{
									echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
								}
							}
						}
					?>
					</td>
                    <td id="div_objetivo_ppa" align="left" valign="top" style="padding-right:4px;">
                        <?php
                        $sql = "select mppaid from pde.ae_acaoxmetappa where acaid =".$arIds[1];
                        $dados = $db->carregarColuna($sql,'mppaid');
                        $metas = implode(',',$dados);

                        if ($metas)
                        {
                            $sql = "select mppaid as codigo, mppanome as nome, temid, o.objid, mppatipo from pde.ae_metappa m
							inner join pde.ae_objetivoppa o on m.objid = o.objid where mppaid in ($metas) order by mppatipo DESC, mppanome";
                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['objid'].'_'.$data['codigo'].'_'.$data['mppatipo'].'" class="tile1 tile4 azul clickMetasPPA">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                                }
                            }
                        }
                        ?>
                    </td>

					<td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;">
						<?php
                        /*
						$sql = "select desid from pde.ae_acaoxdesafio where acaid =".$arIds[1];
						$dados = $db->carregarColuna($sql,'desid');
						$desafios = implode(',',$dados);

						if ($desafios)
						{
							$sql = "select desid as codigo, desnome as nome, o.opeid, temid from pde.ae_desafio d
									inner join pde.ae_objetivope o on d.opeid = o.opeid where desid in ($desafios) order by desnome";
							$rs = $db->carregar($sql);

							if ($rs)
							{
								foreach($rs as $data)
								{
									echo '<div id="'.$data['temid'].'_'.$data['opeid'].'_'.$data['codigo'].'" class="tile1 tile4 amarelo clickDesafiosPE">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
								}
							}
						}
                        */
                        $sql = "select obeid from pde.ae_objetivoestrategicoxacao where acaid =".$arIds[1];
                        $dados = $db->carregarColuna($sql,'obeid');
                        $objetivos = implode(',',$dados);

                        if ($objetivos)
                        {
                            $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($objetivos) order by obenome";
                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 amarelo clickObjetivoEstrategico">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                                }
                            }
                        }
						?>
					</td>
					<td id="div_orcamentos" align="left" valign="top" style="padding-right:4px;">
						<?php	
						$sql = "SELECT vacid, vaetituloorcamentario AS nome
								FROM planacomorc.vinculacaoacaoestrategica
								WHERE acaid = ".$arIds[1]. "
								ORDER BY nome";
						$rs = $db->carregar($sql);
						if ($rs) {
							foreach ($rs as $data) {
								echo '
									<div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');">
										<span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
									</div>
								';
							}
						}
						?>
					</td>
                    <td id="div_projetos" align="left" valign="top" style="padding-right:4px;">
                        <?php
                        $sql = "select solid from pto.acaosolucao where acaid =".$arIds[1];
                        $dados = $db->carregarColuna($sql,'solid');
                        $solucoes = implode(',',$dados);

                        if ($solucoes)
                        {
                            $sql = "SELECT DISTINCT tes.temid, sol.solid AS codigo, COALESCE(sol.solapelido,sol.soldsc) AS nome, sol.solordem
                                    FROM pto.solucao sol
                                    INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                                    WHERE sol.solstatus = 'A'
                                    AND sol.solid IN ($solucoes)
                                    ORDER BY sol.solordem";
                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 marromClaro clickProjetos">
                                            <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
						  		 		    <span class="textoDescricao">'.$data['nome'].'</span><br/>
						  		 		  </div>';
                                }
                            }
                        }
                        ?>
                    </td>
				</tr>
			</table>
			</td>
			</tr>
		</table>
	</div>
	<?php
}

function montaGridPRO()
{
    global $db;
    $arIds = explode('_', $_REQUEST['id']);
?>
    <div class="pagina" style="width:100%">
        <table width="100%" cellpadding="4">
            <tr>
                <td bgcolor="#4682B4" width="50%"><span class="titulo">Estratégias</span></td>
                <td bgcolor="#4682B4" width="50%"><span class="titulo">Iniciativas PEI</span></td>
            </tr>
            <tr>
                <td align="left" valign="top">
                    <div id="div_roll_pagina">
                        <?php
                        $sql = "SELECT ini.estnome
                                FROM pde.ae_estrategia ini
                                INNER JOIN pto.estrategiasolucao ins ON ins.estid = ini.estid
                                WHERE ins.solid = ".$arIds[1]. "
                                ORDER BY ini.estordem";
                        $rs = $db->carregar($sql);
                        if ($rs) {
                            foreach($rs as $data){ ?>
                                <div class="tile1 tile4 azulClaro estrategiaPEI" style="cursor: default">
                                    <span class="textoDescricao"><?php echo $data['estnome']; ?></span><br/>
                                </div>
                            <?php }
                        } ?>
                    </div>
                </td>
                <td align="left" valign="top">
                    <div id="div_roll_pagina">
                        <?php
                        $sql = "SELECT ini.ininome
                                FROM pde.ae_iniciativa ini
                                INNER JOIN pto.iniciativasolucao ins ON ins.iniid = ini.iniid
                                WHERE ins.solid = ".$arIds[1]. "
                                ORDER BY ini.ininome";
                        $rs = $db->carregar($sql);
                        if ($rs) {
                            foreach($rs as $data){ ?>
                                <div class="tile1 tile4 azulClaro iniciativasPEI" style="cursor: default">
                                    <span class="textoDescricao"><?php echo $data['ininome']; ?></span><br/>
                                </div>
                            <?php }
                        } ?>
                    </div>
                </td>
            </tr>
        </table>
        <table width="100%">
            <tr>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas PNE</span></td>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas / Iniciativas PPA</span></td>
                <!--<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos e Desafios</span></td>-->
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos</span></td>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Ações Estratégicas</span></td>
                <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Orçamentos</span></td>
            </tr>
            <tr>
                <td id="div_metas_pne" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select mpneid from pto.metasolucao where solid =".$arIds[1];
                    $dados = $db->carregarColuna($sql,'mpneid');
                    $metas = implode(',',$dados);
                    if ($metas)
                    {
                        $sql = "select
										m.temid,
										m.mpneid as codigo,
										coalesce(ind.link,'') || m.mpnenome as nome
									from pde.ae_metapne m
									left join
										(
										select
											'<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
											mpneid
										from pde.ae_metapnexindicador group by mpneid
										) as ind ON ind.mpneid = m.mpneid
									where m.mpneid in ($metas)
									and m.mpneid <> 99999999
									order by m.mpneordem";
                        $rs = $db->carregar($sql);

                        if ($rs)
                        {
                            foreach($rs as $data)
                            {
                                echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                            }
                        }
                    }
                    ?>
                </td>
                <td id="div_objetivo_ppa" align="left" valign="top" style="padding-right:4px;">

                </td>
                <td id="div_objetivos_estrategicos" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select obeid from pto.objetivosolucao where solid =".$arIds[1];
                    $dados = $db->carregarColuna($sql,'obeid');
                    $objetivos = implode(',',$dados);

                    if ($objetivos)
                    {
                        $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($objetivos) order by obenome";
                        $rs = $db->carregar($sql);

                        if ($rs)
                        {
                            foreach($rs as $data)
                            {
                                echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 amarelo clickObjetivoEstrategico">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                            }
                        }
                    }
                    ?>
                </td>
                <td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select acaid from pto.acaosolucao where solid =".$arIds[1];
                    $dados = $db->carregarColuna($sql, 'acaid');
                    $acoes = implode(',', $dados);

                    if ($acoes) {
                        $sql = "
                                select aca.temid, aca.acaid as codigo, aca.acadsc as nome, aca.acadetalhe, aca.indidprincipal, coc.coclink
                                from painel.acao aca
                                left join pde.cockpit coc ON coc.cocid = aca.cocid
                                where aca.acaid in ($acoes)
                                order by aca.acadsc
                            ";
                        $rs = $db->carregar($sql);
                        if ($rs) {
                            foreach ($rs as $data) {
                                echo '
                                        <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['codigo'] . '" >
                                            ' . ($data['coclink'] ? '<a href="' . $data['coclink'] . '" target="_blank" class="linkPopupPainel" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '') . '
                                            ' . ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid=' . $data['indidprincipal'] . '" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '') . '
                                            <a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid=' . $data['codigo'] . '" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;
                                            <span class="textoDescricao" title="' . $data['acadetalhe'] . '" alt="' . $data['acadetalhe'] . '">' . $data['nome'] . '</span><br/>
                                        </div>
                                    ';
                            }
                        }
                    }
                    ?>
                </td>
                <td id="div_orcamentos" align="left" valign="top" style="padding-right:4px;">
                    <?php
                    $sql = "select acaid from pto.acaosolucao where solid =".$arIds[1];
                    $dados = $db->carregarColuna($sql, 'acaid');
                    $orcamentos = implode(',', $dados);

                    if ($orcamentos) {
                        $sql = "SELECT vacid, vaetituloorcamentario AS nome
                                FROM planacomorc.vinculacaoacaoestrategica
                                WHERE acaid in ($orcamentos)
                                ORDER BY nome";
                        $rs = $db->carregar($sql);
                        if ($rs) {
                            foreach ($rs as $data) {
                                echo '
                                    <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');">
                                        <span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                                    </div>
                                ';
                            }
                        }
                    }
                    ?>
                </td>
        </table>
    </div>
<?php
}

function montaGridOrc()
{
	global $db;
	$arIds = explode('_', $_REQUEST['id']);
	$sql = "SELECT sba.descricao FROM planacomorc.subacao sba WHERE sba.codigo = '{$arIds[1]}'";
	$rs = $db->carregar($sql);
	?>
	<div class="pagina" style="width:100%">
		<table width="100%" cellpadding="3">
			<tr>
				<td  bgcolor="#4682B4"><span class="titulo">Detalhe do Orçamento</span></td>
			</tr>
			<tr>
				<td colspan="3">
					<div id="div_roll_pagina">
						<div class="tile1  tile4 azulClaro orcamentos" style="cursor: default">
						<?
						if ($rs)
						{
							 echo $rs[0]['descricao'];
						}
						?>
						</div>
					</div>
				</td>
			</tr>
			<tr>
			<td>
			<table width="100%">
				<tr>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas PNE</span></td>
                    <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas / Iniciativas PPA</span></td>
					<!--<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos e Desafios</span></td>-->
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Ações Estratégicas</span></td>
					<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Projetos</span></td>
				</tr>
				<tr>
					<td id="div_metas_pne" align="left" valign="top" style="padding-right:4px;">
					<?php
						$sql = "SELECT DISTINCT apn.mpneid
								FROM planacomorc.vinculacaoestrategicasubacoes ves
								INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
								INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
								INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
								INNER JOIN pde.ae_acaoxmetapne apn ON apn.acaid = vac.acaid
								WHERE sba.codigo = '{$arIds[1]}'";
						$dados = $db->carregarColuna($sql,'mpneid');
						$metas = implode(',',$dados);

						if ($metas)
						{
							$sql = "select 
										m.temid, 
										m.mpneid as codigo, 
										coalesce(ind.link,'') || m.mpnenome as nome 
									from pde.ae_metapne m
									left join
										(
										select
											'<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
											mpneid
										from pde.ae_metapnexindicador group by mpneid
										) as ind ON ind.mpneid = m.mpneid
									where m.mpneid in ($metas)
									and m.mpneid <> 99999999
									order by m.mpneordem";
							$rs = $db->carregar($sql);

							if ($rs)
							{
								foreach($rs as $data)
								{
									echo '<div  id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
								}
							}
						}
					?>
					</td>
                    <td id="div_objetivo_ppa" align="left" valign="top" style="padding-right:4px;">
                        <?php
                        $sql = "SELECT DISTINCT app.mppaid
								FROM planacomorc.vinculacaoestrategicasubacoes ves
								INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
								INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
								INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
								INNER JOIN pde.ae_acaoxmetappa app ON app.acaid = vac.acaid
								WHERE sba.codigo = '{$arIds[1]}'";
                        $dados = $db->carregarColuna($sql,'mppaid');
                        $metas = implode(',',$dados);

                        if ($metas)
                        {
                            $sql = "select mppaid as codigo, mppanome as nome, temid, o.objid, mppatipo from pde.ae_metappa
									m  inner join pde.ae_objetivoppa o on m.objid = o.objid where mppaid in ($metas) order by mppatipo DESC, mppanome";
                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['objid'].'_'.$data['codigo'].'_'.$data['mppatipo'].'" class="tile1 tile4 azul clickMetasPPA">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                                }
                            }
                        }
                        ?>
                    </td>
					<td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;">
						<?php
                        /*
						$sql = "SELECT DISTINCT ade.desid
								FROM planacomorc.vinculacaoestrategicasubacoes ves
								INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
								INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
								INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
								INNER JOIN pde.ae_acaoxdesafio ade ON ade.acaid = vac.acaid
								WHERE sba.codigo = '{$arIds[1]}'";
						$dados = $db->carregarColuna($sql,'desid');
						$desafios = implode(',',$dados);

						if ($desafios)
						{
							$sql = "select desid as codigo, desnome as nome, temid, o.opeid from pde.ae_desafio d
									inner join pde.ae_objetivope o on d.opeid = o.opeid  where desid in ($desafios) order by desnome";

							$rs = $db->carregar($sql);

							if ($rs)
							{
								foreach($rs as $data)
								{
									echo '<div id="'.$data['temid'].'_'.$data['opeid'].'_'.$data['codigo'].'"  class="tile1 tile4 amarelo clickDesafiosPE">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
								}
							}
						}
                        */
                        $sql = "SELECT DISTINCT ade.obeid
								FROM planacomorc.vinculacaoestrategicasubacoes ves
								INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
								INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
								INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
								INNER JOIN pde.ae_objetivoestrategicoxacao ade ON ade.acaid = vac.acaid
								WHERE sba.codigo = '{$arIds[1]}'";
                        $dados = $db->carregarColuna($sql,'obeid');
                        $desafios = implode(',',$dados);

                        if ($desafios)
                        {
                            $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($desafios) order by obenome";

                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'"  class="tile1 tile4 amarelo clickObjetivoEstrategico">
						  		 		  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                                }
                            }
                        }
						?>
					</td>
					<td id="div_orcamentos" align="left" valign="top" style="padding-right:4px;">
					<?php
						$sql = "SELECT DISTINCT vac.acaid
								FROM planacomorc.vinculacaoestrategicasubacoes ves
								INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
								INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
								INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
								WHERE sba.codigo = '{$arIds[1]}'";
						$dados = $db->carregarColuna($sql,'acaid');
						$acoes = implode(',',$dados);

						if ($acoes)
						{
							$sql = "select aca.temid, aca.acaid, aca.acadsc, aca.indidprincipal, coc.coclink
									from painel.acao aca
									left join pde.cockpit coc ON coc.cocid = aca.cocid
									where aca.acaid in ($acoes)
									order by aca.acadsc";
							$rs = $db->carregar($sql);

							if ($rs)
							{
								foreach($rs as $data)
								{
									echo ('<div id="'.$data['temid'].'_'.$data['acaid'].'" class="tile1 tile4 laranja clickAcoesEstrategicas">');
									echo ($data['coclink'] ? '<a href="'.$data['coclink'].'" class="linkPopupPainel" target="_blank" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>':'');
									echo ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid='.$data['indidprincipal'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '');
									echo ('<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid='.$data['acaid'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;' . $data['acadsc']);
									echo '</div>';
								}
							}
						}
						?>
					</td>
                    <td id="div_projetos" align="left" valign="top" style="padding-right:4px;">
                        <?php
                        $sql = "SELECT DISTINCT acs.solid
								FROM planacomorc.vinculacaoestrategicasubacoes ves
								INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
								INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
								INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
								INNER JOIN pto.acaosolucao acs ON acs.acaid = vac.acaid
								WHERE sba.codigo = '{$arIds[1]}'";
                        $dados = $db->carregarColuna($sql,'solid');
                        $solucoes = implode(',',$dados);

                        if ($solucoes)
                        {
                            $sql = "SELECT DISTINCT tes.temid, sol.solid AS codigo, COALESCE(sol.solapelido,sol.soldsc) AS nome, sol.solordem
                                    FROM pto.solucao sol
                                    INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                                    WHERE sol.solstatus = 'A'
                                    AND sol.solid IN ($solucoes)
                                    ORDER BY sol.solordem";
                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'"  class="tile1 tile4 marromClaro clickProjetos">
                                                <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
						  		 		        <span class="textoDescricao">'.$data['nome'].'</span><br/>
						  		 		  </div>';
                                }
                            }
                        }
                        ?>
                    </td>
				</tr>
			</table>
			</td>
			</tr>
		</table>
	</div>
	<?php
}

function montaGridPNE(){
	?>
	<div class="pagina" style="width:100%">
		<table width="100%" cellpadding="3">
			<tr>
				<td colspan="5" bgcolor="#4682B4"><span class="titulo">Estratégias</span></td>
			</tr>
			<tr>
				<td colspan="5">
					<div id="div_roll_pagina">
						<div id="td_estrategias_pne">
						</div>
					</div>
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="20%" bgcolor="#4682B4"><span class="titulo">Metas / Iniciativas PPA</span></td>
				<!--<td align="left" valign="top" width="20%" bgcolor="#4682B4"><span class="titulo">Objetivos Estratégicos e Desafios</span></td>-->
				<td align="left" valign="top" width="20%" bgcolor="#4682B4"><span class="titulo">Objetivos Estratégicos</span></td>
				<td align="left" valign="top" width="20%" bgcolor="#4682B4"><span class="titulo">Ações Estratégicas</span></td>
				<td align="left" valign="top" width="20%" bgcolor="#4682B4"><span class="titulo">Orçamentos</span></td>
				<td align="left" valign="top" width="20%" bgcolor="#4682B4"><span class="titulo">Projetos</span></td>
			</tr>
			<tr>
				<td id="td_objetivos_ppa" align="left" valign="top"></td>
				<!--<td id="td_planejamento_estrategico" align="left" valign="top"></td>-->
				<td id="td_objetivo_estrategico_pne" align="left" valign="top"></td>
				<td id="td_acoes_metas" align="left" valign="top" ></td>
				<td id="td_orcamentos" align="left" valign="top" ></td>
				<td id="td_projetos_pne" align="left" valign="top" ></td>
			</tr>
		</table>
	</div>

<?PHP

    function delimitador($texto){
        if(strlen($texto) > 120){
                $texto = substr($texto,0,120).'...';
        }
        return $texto;
    }

}

function verificarPerfilParaAcessoModulo(){
    global $db;

    $perfil = pegaPerfilGeral();

    if( !( in_array(PERFIL_SUPER_USUARIO, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil)) ){
        if ( in_array(PERFIL_CONSULTA_EXTERNA, $perfil) ){
            die('A'); #"MODULO" ALINHAMENTO
        }else{
            die('T'); #"MODULO" ALINHAMENTO E PLANEJAMENTO - TODOS.
        }
    }else{
        die('T'); #"MODULO" ALINHAMENTO E PLANEJAMENTO - TODOS.
    }
}


?>

<script src="../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>

<script>
    $('#busca').bt({
        trigger: 'none',
        contentSelector: "$('#conteudotip')",
        width: 200,
        shadow: true,
        cssStyles: {
            color: 'gray',
            fontWeight: 'bold'
        },
        shadowColor: 'rgba(0,0,0,.5)',
        shadowBlur: 8,
        shadowOffsetX: 4,
        shadowOffsetY: 4,
        positions: ['left', 'right', 'bottom']
    });

    $('document').ready(function(){

        var abrirModulo;

        abrirModulo = verificarPerfilParaAcessoModulo();

        if( abrirModulo == 'A' ){
            $('#td_acoes_estrategicas').hide();

            $('#divalinhamento').show();
            $('#divplanejamento').hide();
        }else{
            $('#td_acoes_estrategicas').show();
            $('#td_alinhamento_estrategico').show();

            $('#divplanejamento').show();
            $('#divalinhamento').hide();
        }

        $('#busca').btOn();
        window.setTimeout("$('#busca').btOff()", 7000);

        $('#div-acoes-estrategicas').click(function(){
            $('#div-acoes-estrategicas').css("background-color","gray");
            $('#div-alinhamento-estrategico').css("background-color","navy");
        });

        $('#div-alinhamento-estrategico').click(function(){
            $('#div-acoes-estrategicas').css("background-color","navy");
            $('#div-alinhamento-estrategico').css("background-color","gray");
        });

        <? if($_REQUEST['chamadaExterna']){ ?>
                area       = '<?=$_REQUEST['area']?>';
                eixo       = '<?=$_REQUEST['eixo']?>';
                qdPrincipal= '<?=$_REQUEST['qdPrincipal']?>';
                qdSuperior = '<?=$_REQUEST['qdSuperior']?>';
                tipo       = '<?=$_REQUEST['tipo']?>';

                $('#div-alinhamento-estrategico').trigger('click');
                $('.eixo').show();

                if (area == 'ppa')
                    selecionaMetaPPA(eixo, qdPrincipal, qdSuperior,tipo );

                if (area == 'pne')
                    selecionaMetaPNE(eixo, qdPrincipal);

                if (area == 'orc')
                    selecionaOrcamento(eixo, qdPrincipal);

                if (area == 'pro')
                    selecionaProjeto(eixo, qdPrincipal);

                if (area == 'ae')
                    selecionaAcao(eixo, qdPrincipal);

                if (area == 'pe')
                    selecionaDesafioPE(eixo, qdPrincipal,qdSuperior);

                if (area == 'obj')
                    selecionaObjetivoEstrategico(eixo, qdPrincipal);

        <? } ?>
    });

    function verificarPerfilParaAcessoModulo(){
        var resp;

        $.ajax({
            type : "POST",
            url  : window.location,
            data : {
                requisicao : 'verificarPerfilParaAcessoModulo'
            },
            async: false,
            success: function(dados){
                resp = dados;
            }
        });

        return resp;
    }
	
	function abreOrcamento(vacid)
	{
		window.open('/planacomorc/planacomorc.php?modulo=sistema/tabelasapoio/popupImpressaoVinculacaoOrcamentaria&acao=A&vacid='+vacid,'blank','height=500,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}
</script>
