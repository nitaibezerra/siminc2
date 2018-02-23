<?php

class AlinhamentoEstrategico
{
    public function carregarTemaPormeta($mpneid){
        $db = new cls_banco();
        $sql = "select temid from pde.ae_metapne m where mpneid = $mpneid";
        $rs = $db->pegaLinha($sql);
        echo ($rs["temid"]);
    }
    public function carregarMetas($area,$temid){
        $db = new cls_banco();
        
        switch ($area) {
            case 'pe':
                $sql = "select opeid as codigo, openome as descricao from pde.ae_objetivope
                                            where temid = {$temid} order by openome";
                $subTitulo = "Objetivos Estratégicos";
                break;
            case 'obj':
                $sql = "select obeid as codigo, obenome as descricao from pde.ae_objetivoestrategico
                                            where temid = {$temid} order by obeordem";
                $subTitulo = "Objetivos Estratégicos";
                break;
            case 'ppa':
                $sql = "select objid as codigo, objnome as descricao from pde.ae_objetivoppa
                                            where temid = {$temid} order by objordem";
                $subTitulo = "Objetivos";
                break;
            case 'pne':
                            $sql = "select
                                                    m.mpneid as codigo,
                                                    CASE m.mpneid
                                                        WHEN 99999999 THEN coalesce(ind.link,'') || m.mpnenome
                                                    ELSE
                                coalesce(ind.link,'') || m.mpnenome || '<br><br>' ||
                                '<table border=''2'' align=''center'' width=''98%'' cellspacing=''4'' cellpadding=''5''>
                                    <tr>
                                        <td width=''20%''>Prazo</td><td width=''70%''>Encaminhamento</td><td width=''5%''>Situação</td><td width=''5%''>Crítico?</td>
                                    </tr>
                                    <tr>
                                        <td>'||to_char(m.mpneprazo, 'DD/MM/YYYY')||'</td>
                                        <td>'|| COALESCE((SELECT ecp.ecpdsc FROM pde.ae_encaminhamentopne ecp WHERE ecp.mpneid = m.mpneid ORDER BY ecpdata DESC, ecpid DESC LIMIT 1),'-') || '</td>
                                        <td align=''center''>' ||
                                            CASE m.sitid
                                                WHEN 1 THEN '<img src=''../imagens/erro_checklist.png'' height=''30px'' width=''30px'' title='''|| sit.sitdsc ||'''>'
                                                WHEN 2 THEN '<img src=''../imagens/lapis2.png'' height=''30px'' width=''30px'' title='''|| sit.sitdsc ||'''>'
                                                WHEN 3 THEN '<img src=''../imagens/check_checklist.png'' height=''30px'' width=''30px'' title='''|| sit.sitdsc ||'''>'
                                            END ||
                                        '</td>
                                        <td align=''center''>'||
                                            CASE
                                                WHEN m.mpnecritico IS TRUE THEN '<img src=''../imagens/obras/atencao.png'' height=''30px'' width=''30px'' title=''Crítico''>'
                                                ELSE 'NÃO'
                                            END ||
                                        '</td>
                                    </tr>
                                </table>'
                                                    END as descricao
                                            from pde.ae_metapne m
                                            left join pde.ae_situacao sit ON sit.sitid = m.sitid
                                            left join
                                                    (
                                                    select
                                                            '<a href=''/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&paiid=1&mpneid='|| mpneid ||''' target=''_blank'' class=''linkPopupIndicador'' alt=''Indicadores'' title=''Indicadores''><img src=''../imagens/eixos-mini2.png'' height=''15'' border=''0'' /></a>&nbsp;' AS link,
                                                            mpneid
                                                    from pde.ae_metapnexindicador group by mpneid
                                                    ) as ind ON ind.mpneid = m.mpneid
                                            where m.temid = {$temid}
                                            --and m.mpneid <> 99999999
                                            order by m.mpneordem";
                $subTitulo = "Metas";
                break;
            case 'ae':
                $sql = "select acaid as codigo, acadsc as descricao from painel.acao
                                    where acastatus = 'A' AND temid = {$temid}  order by acadsc ";
                $subTitulo = "Ações";
                break;
            case 'orc':
                            $sql = "SELECT DISTINCT sba.codigo AS codigo, sba.codigo || ' - ' || sba.titulo AS descricao
                                            FROM planacomorc.vinculacaoestrategicasubacoes ves
                                            INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
                                            INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
                                            INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
                                            INNER JOIN painel.acao aca ON aca.acaid = vac.acaid
                                            WHERE aca.temid = {$temid}
                                            ORDER BY descricao";
                $subTitulo = "Subações Orçamentárias";
                break;
            case 'pro':
                $sql = "SELECT
                            DISTINCT sol.solid AS codigo,
                            '<a href=''/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='|| sol.solid ||''' target=''_blank''><div class=''glyphicon glyphicon-zoom-in'' height=''30px'' width=''30px'' title=''Detalhar Projeto''></div></a>&nbsp;' || COALESCE(sol.solapelido,sol.soldsc) AS descricao,
                            sol.solordem
                        FROM pto.solucao sol
                        INNER JOIN pto.temasolucao tes ON tes.solid = sol.solid
                        WHERE sol.solstatus = 'A'
                        AND tes.temid = {$temid}
                        ORDER BY sol.solordem";
                $subTitulo = "Projetos";
                break;
        }
        if ($sql) {
            $rs = $db->carregar($sql);

                echo "<table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>";
                echo "    <tr>";
                echo "        <td style='font-size: 20px; text-align:center; font-weight:bold; background-color: #E4D779;' class='tituloAno' >";
                echo $subTitulo;
                echo "        </td>";
                echo "    </tr>";
                    if ($rs) {
                        echo "";
                        echo "    <tr>";
                        echo "    <td>";
                            echo '<div id="div_lnk_objetivos_metas">';
                             if ($area != 'pne' && $area != 'pro'){
                                echo '<div class="row textoDescricao">';
                                foreach ($rs as $dados) {
                                    echo '<div class="col-md-3">';
                                    echo '<div title="'.$dados['descricao'].'"id="' . $area . '_' . $dados['codigo'] . '"  class="widget p-xl objetivos_metas_todos" style="border: 1px solid #273A4A;cursor: pointer;padding:10px;height:80px;"> ';
                                    echo  '<ul class="list-unstyled m-t-md" style="margin:auto">
                                            <li>'.substr($dados['descricao'],0,100) . ' ...' ;
                                    echo '</li>';
                                    echo '</ul>';
                                    echo '</div>';
                                    echo '</div>';
                                }
                                echo '</div>';
                            } else{
                                foreach ($rs as $dados) {
                                    echo '<div id="' . $area . '_' . $dados['codigo'] . '"  class="objetivos_metas " style="margin-bottom: 5px; cursor: pointer;"> ' . $dados['descricao'] . '</div>';
                                }
                            }
                            echo '</div>';
                        echo "    </td>";
                        echo "    </tr>";
                        echo "    </table>";
                    }
            echo "    </td>";
            echo "    </tr>";
            echo "    </table>";
        } else {
            echo "Sem registros.";
        }

    }
    public function montarMetas($temid = null)
    {
        $db = new cls_banco();
?>
        <div style="text-align:center;">
            <div class="btn-group" id="metas" data-toggle="buttons" style="text-align: center;">
                <?php
                unset($temid);
                if (!empty($temid)){
                    $where = " where temid = $temid";
                }
                $sql = "SELECT mpneid, CASE WHEN mpneid <> 99999999 THEN Replace(SUBSTR(mpnenome,1,7),':','') ELSE mpnenome END AS descricao,mpapelido FROM pde.ae_metapne $where ORDER BY mpneordem";
                $arrDados = $db->carregar($sql);
                $arrDados = $arrDados ? $arrDados : array();
                foreach ($arrDados as $dados) {
                    $active = is_array($_POST['estuf']) && in_array($dados['estuf'], $_POST['estuf']);
                    ?>
                    <div id="metas_label_<?php echo $dados['mpneid'];?>" style="font-size: 10px; " class="btn btn-navy goal-item isotope-item metas  <?php echo $active ? 'active' : ''; ?>">
                        <input type="radio" id="radio-meta" name="radio-metas" class="radio-metas" value="<?php echo $dados['mpneid'];?>" <?php echo $active ? 'checradioked="checked"' : ''; ?>><?php echo $dados['descricao'];?>
                    </div>
                <?php } ?>
            </div>
        </div>
<?php
    }
    public function montarArea(){
?>
        <div style="text-align:center;">
            <div class="btn-group " data-toggle="buttons">
                <label style="font-size: 10px" class="btn btn-navy goal-item isotope-item area <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="area"  id="area-pne" value="pne" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Plano Nacional de Educação
                </label>
                <label style="font-size: 10px" class="btn btn-navy goal-item isotope-item area <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="area" id="area-ppa" value="ppa" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Plano Plurianual
                </label>
                <label style="font-size: 10px" class="btn btn-navy goal-item isotope-item area <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="area" id="area-obj" value="obj" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Planejamento Estratégico
                </label>
                <label style="font-size: 10px" class="btn btn-navy goal-item isotope-item area <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="area" id="area-ae" value="ae" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Ações Estratégicas
                </label>
                <label style="font-size: 10px" class="btn btn-navy goal-item isotope-item area <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="area" id="area-orc" value="orc" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Orçamento
                </label>
                <!--label style="font-size: 10px" class="btn btn-navy goal-item isotope-item area <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="area" id="area-pro" value="pro" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Projetos
                </label-->
            </div>
        </div>
<?php
    }
    public function montarTema(){
?>
        <div style="text-align:center;">
            <div class="btn-group" data-toggle="buttons">
                <label id="temas-label-1" style="font-size: 10px" class="btn btn-navy goal-item isotope-item temid <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="temid"  id="temid-1" value="1" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Acesso e Qualidade da Educação Básica
                </label>
                <label id="temas-label-2" style="font-size: 10px" class="btn btn-navy goal-item isotope-item temid <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="temid" id="temid-2" value="2" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Educação Profissional e Tecnológica
                </label>
                <label id="temas-label-3" style="font-size: 10px" class="btn btn-navy goal-item isotope-item temid <?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="temid" id="temid-3" value="3" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Educação Superior
                </label>
                <label id="temas-label-4" style="font-size: 10px" class="btn btn-navy goal-item isotope-item temid<?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="temid" id="temid-4" value="4" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Planejamento e Gestão das Políticas Educacionais
                </label>
                <label id="temas-label-5" style="font-size: 10px" class="btn btn-navy goal-item isotope-item temid<?php echo $active ? 'active' : ''; ?>">
                    <input type="radio" name="temid" id="temid-5" value="5" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Formação e Valorização de Professores e Profissionais da Educação
                </label>

                    <label id="temas-label-6" style="font-size: 10px" class="btn btn-navy goal-item isotope-item temid<?php echo $active ? 'active' : ''; ?>">
                        <input type="radio" name="temid" id="temid-6" value="6" <?php echo $active ? 'checradioked="checked"' : ''; ?>>Suporte
                    </label>

            </div>
        </div>
<?php
    }
    public function montaGridPNE($id){
        global $db;
        $arIds = explode('_', $id);
            ?>
            <table border='2'  align='left' width='80%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                <tr>
                    <?php
                    $sql = "SELECT mpneid, mpnenome FROM pde.ae_metapne WHERE mpneid = {$arIds[1]}";
                    $arrMeta = $db->pegaLinha($sql);
                    echo "<td colspan='5' style='font-size: 20px; text-align:left; font-weight:bold;' class='tituloAno'>";
                    echo "<a href='/pde/graficopne.php?planejamento=sim&metid=".$arrMeta['mpneid']."' target='_blank' title='Detalhe da Meta'><img src='../imagens/seriehistorica_ativa.gif' height='15' border='0' /></a>&nbsp;" . $arrMeta['mpnenome'];
                    echo "</td>";
                    ?>
                </tr>
            </table>
            <div class="col-md-6">
                    <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                        <thead>
                            <tr>
                                <?php
                                if($arIds[1] != 99999999){
                                    echo "<td colspan='5' style='font-size: 20px; text-align:center; font-weight:bold; background-color: #E4D779;' class='tituloAno'>Estratégias</td>";
                                }else{
                                    echo "<td colspan='5' bgcolor='#4682B4' class='titulo'>Artigos</td>";
                                }
                                ?>
                            </tr>
                        </thead>
                            <tr>
                                    <td colspan="5">
                                            <div id="div_roll_pagina" style="margin: 0 auto; padding: 0; height: 250px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
                                                    <div id="td_estrategias_pne">
                                                    </div>
                                            </div>
                                    </td>
                            </tr>
                    </table>
            </div>
            <div class="col-md-6">
                <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                    <thead>
                        <tr>
                                <td align="left" valign="top" width="20%"  class="titulo">Metas / Iniciativas PPA</td>
                                <!--<td align="left" valign="top" width="20%" class="titulo">Objetivos Estratégicos e Desafios</td>-->
                                <td align="left" valign="top" width="20%" class="titulo">Objetivos Estratégicos</td>
                                <td align="left" valign="top" width="20%" class="titulo">Ações Estratégicas</td>
                                <td align="left" valign="top" width="20%" class="titulo">Orçamentos</td>
                                <!--td align="left" valign="top" width="20%" class="titulo">Projetos</td-->
                        </tr>
                    </thead>
                    <tr>
                        <td colspan="4">
                            <div style="margin: 0 auto; padding: 0; height: 260px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
                                <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                    <tr>
                                        <td id="td_objetivos_ppa" align="left" valign="top" width="20%"></td>
                                        <!--<td id="td_planejamento_estrategico" align="left" valign="top" width="20%"></td>-->
                                        <td id="td_objetivo_estrategico_pne" align="left" valign="top" width="21%"></td>
                                        <td id="td_acoes_metas" align="left" valign="top" width="21%"></td>
                                        <td id="td_orcamentos" align="left" valign="top" width="18%"></td>
                                        <!--td id="td_projetos_pne" align="left" valign="top" width="20%"></td-->
                                    </tr>
                                </table>
                            </div>
                        </td>
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
    public function carregaEstrategiasPNE($id){
            $db = new cls_banco();
            if ($id > 0) {
                if($id != 99999999){
                    $sql = "select estnome as nome, to_char(est.estprazo,'DD/MM/YYYY') AS prazo, sit.sitdsc, est.estcritico AS critico,
                                (SELECT ece.ecedsc FROM pde.ae_encaminhamentoestrategia ece WHERE ece.estid = est.estid ORDER BY ecedata DESC, eceid DESC LIMIT 1) AS encaminhamento
                            from pde.ae_estrategia est
                            INNER JOIN pde.ae_situacao sit ON sit.sitid = est.sitid
                            where metid = " . $id . " ORDER BY estordem";
                }else{
                    $sql = "SELECT art.artnome as nome, to_char(art.artprazo,'DD/MM/YYYY') AS prazo, sit.sitdsc, art.artcritico AS critico,
                                (SELECT eca.ecadsc FROM pde.ae_encaminhamentoartigo eca WHERE eca.artid = art.artid ORDER BY ecadata DESC, ecaid DESC LIMIT 1) AS encaminhamento
                            FROM pde.ae_artigo art
                            INNER JOIN pde.ae_situacao sit ON sit.sitid = art.sitid
                            ORDER BY art.artordem";
                }
                $rs = $db->carregar($sql);
                if ($rs) {
                    foreach ($rs as $data) {
                        echo '
                            <div class="tile4 " style="cursor: default;border-bottom: 1px #CCC solid;">
                                <span class="textoDescricao">' . $data['nome'] . '</span><br/><br/>
                                
                            </div>
                        ';
                }
            }
        }
    }
    public function carregaMetaPPA($id){
        $db = new cls_banco();
        if ($id > 0) {
            $sql = "select mppaid from pde.ae_metapnexppa where mpneid = {$id}";
            $dados = $db->carregarColuna($sql, 'mppaid');
            $metas = implode(',', $dados);

            if ($metas) {
                $sql = "
                    select mppaid as codigo, COALESCE(mppanomeresumido,mppanome) as nome, mppanome as nomecompleto, temid,o.objid, mppatipo from pde.ae_metappa
                    m  inner join pde.ae_objetivoppa o on m.objid = o.objid
                    where mppaid in ($metas) order by mppaordem
                ";
                $rs = $db->carregar($sql);

                if ($rs) {
                    foreach ($rs as $data) {
                        echo '
                            <div id="' . $data['temid'] . '_' . $data['objid'] . '_' . $data['codigo'] . '_' . $data['mppatipo'] . '"  class="tile1 tile4 azul clickMetasPPA" style="padding: 0 0 20 0;">
                                <span class="textoDescricao" title="'.$data['nomecompleto'].'">' . $data['nome'] . '</span><br/>
                            </div>
                        ';
                    }
                }
            }
        }

    }
    public function carregarDesafiosPNE($id){
        $db = new cls_banco();
        if ($id > 0) {
            $sql = "select desid from pde.ae_desafioxmetapne where mpneid = " .$id ;
            $dados = $db->carregarColuna($sql, 'desid');
            $desafios = implode(',', $dados);

            if ($desafios) {
                $sql = "select desid as codigo, desnome as nome, temid, o.opeid from pde.ae_desafio d
                            inner join pde.ae_objetivope o on d.opeid = o.opeid where desid in ($desafios) order by desnome";
                $rs = $db->carregar($sql);

                if ($rs) {
                    foreach ($rs as $data) {
                        echo '
                            <div id="' . $data['temid'] . '_' . $data['opeid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickDesafiosPE" style="padding: 0 0 20 0;">
                                <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
                                        </div>
                        ';
                    }
                }
            }
        }
    }
    public function carregarObjetivosEstrategicosPNE($id){
        $db = new cls_banco();
        if ($id > 0) {
            $sql = "select obeid from pde.ae_objetivoestrategicoxmetapne where mpneid = " . $id;
            $dados = $db->carregarColuna($sql, 'obeid');
            $objetivos = implode(',', $dados);

            if ($objetivos) {
                $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($objetivos) order by obeordem";
                $rs = $db->carregar($sql);

                if ($rs) {
                    foreach ($rs as $data) {
                        echo '
                            <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickObjetivoEstrategico" style="padding: 0 0 20 0;">
                                <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
                                        </div>
                        ';
                    }
                }
            }
        }
    }
    public function carregaAcoesPDE($id){
        $db = new cls_banco();
        if ($id > 0) {
            $sql = "select acaid from pde.ae_acaoxmetapne where mpneid =" . $id;
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
                            <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['codigo'] . '"  style="padding: 0 0 20 0;">
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
    }
    public function carregaOrcamentos($id){
        $db = new cls_banco();
        if ($id > 0) {
                    $sql = "select acaid from pde.ae_acaoxmetapne where mpneid =" . $id;
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
                            <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');" style="padding: 0 0 20 0;">
                                                            <span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                            </div>
                        ';
                    }
                }
            }
        }
    }
    public function carregaProjetosPNE($id){
        $db = new cls_banco();
        if ($id > 0) {
            $sql = "select solid from pto.metasolucao where mpneid = " . $id;
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
                            <div class="tile1 tile4 marromClaro" onclick="abreSolucao('.$data['codigo'].');" style="padding: 0 0 20 0;">
                                <div class="glyphicon glyphicon-zoom-in" height="25px" width="25px" title="Detalhar Projeto"></div>&nbsp;
                                <span class="textoDescricao">' . $data['nome'] . '</span><br/>
                            </div>
                        ';
                    }
                }
            }
        }
    }
    public function montaGridPPA($id){
	global $db;
	$arIds = explode('_', $id);
	$sql = "select mppaid, mppanome AS nome, mppatipo from pde.ae_metappa where objid = {$arIds[1]}";
	$rs = $db->carregar($sql);
	?>
	<?php if($rs): ?>
            <div class="wrapper wrapper-content animated fadeIn">
		<div class="row">
                    <div class="col-md-6"> 
                        <div class="tabs-container">
                            <ul class="nav nav-tabs">
                                <li class="active"><a data-toggle="tab" href="#tab-1" aria-expanded="true"> Metas PPA</a></li>
                                <li class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false">Iniciativas PPA</a></li>
                            </ul>
                            <div class="tab-content">
                                <div id="tab-1" class="tab-pane active" aria-expanded="true">
                                        <div class="panel-body" style="margin: 0 auto; padding: 0; height: 220px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
                                            <?php foreach($rs as $data): ?>
                                                    <?php if($data['mppatipo'] == 'M'): ?>
                                                            <div class="tile1 tile4  iniciativas" style="cursor:pointer;border-bottom: 1px #CCC solid;padding:5px 0 20 0;" id="metas_<?php echo($data['mppaid'])?>">
                                                                <span class="textoDescricao"><?php echo $data['nome']; ?></span><br/>
                                                            </div>
                                                    <?php endif; ?>
                                            <?php endforeach; ?>
                                        </div>
                                </div>
                                <div id="tab-2" class="tab-pane fade" aria-expanded="false">
                                    <div class="panel-body" style="margin: 0 auto; padding: 0; height: 220px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
                                        <?php foreach($rs as $data): ?>
                                                <?php if($data['mppatipo'] == 'I'): ?>
                                                        <div class="tile1 tile4  iniciativas" style="cursor:pointer;border-bottom: 1px #CCC solid;padding:5px 0 20 0;" id="iniciativas_<?php echo($data['mppaid'])?>">
                                                            <span class="textoDescricao"><?php echo $data['nome']; ?></span><br/>
                                                        </div>
                                                <?php endif; ?>
                                        <?php endforeach; ?>
                                    </div>
                                </div>
                            </div>

                        </div>
                    </div>
                    <div class="col-md-6">
			            <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                            <tr>
                                <td bgcolor="#4682B4" width="20%" class="titulo">Metas PNE</td>
                                <!--<td bgcolor="#4682B4" width="20%" class="titulo">Objetivos Estratégicos e Desafios</span></td>-->
                                <td bgcolor="#4682B4" width="20%" class="titulo">Objetivos Estratégicos</td>
                                <td bgcolor="#4682B4" width="20%" class="titulo">Ações Estratégicas</td>
                                <td bgcolor="#4682B4" width="20%" class="titulo">Orçamentos</td>
                                <!--td bgcolor="#4682B4" width="20%" class="titulo">Projetos</td-->
                            </tr>
                            <tr>
                                <td colspan="4">
                                    <div style="margin: 0 auto; padding: 0; height: 210px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
                                        <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                            <tr>
                                                <td id="div_metas_pne" align="left" valign="top" width="20%"></td>
                                                <!--<td id="div_desafios_pe" align="left" valign="top" width="20%"></td>-->
                                                <td id="div_objetivos_estrategicos_pe" align="left" valign="top" width="21%"></td>
                                                <td id="div_acoes_metas" align="left" valign="top" width="21%"></td>
                                                <td id="div_orcamentos" align="left" valign="top" width="18%"></td>
                                                <!--td id="div_projetos_ppa" align="left" valign="top" width="20%"></td--><!-- NÃO VINCULADO-->
                                            </tr>
                                        </table>
                                    </div>
                                </td>
                            </tr>
			            </table>
                    </div>
		</div>
            </div>
	<?php endif; ?>
	<?php
    }
    public function carregaDesafiosPE($id){
        global $db;
        if ($id > 0) {
            $sql = "select desid from pde.ae_desafioxmetappa where mppaid =" . $id;
            $dados = $db->carregarColuna($sql, 'desid');
            $desafios = implode(',', $dados);
            if ($desafios) {
                $sql = "select desid as codigo, desnome as nome, temid, o.opeid from pde.ae_desafio d
                            inner join pde.ae_objetivope o on d.opeid = o.opeid where desid in ($desafios) order by desnome";
                $rs = $db->carregar($sql);
                if ($rs) {
                    foreach ($rs as $data) {
                        echo '
                            <div id="' . $data['temid'] . '_' . $data['opeid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickDesafiosPE" style="padding: 0 0 20 0;">
                                <span class="textoDescricao">' . $data['nome'] . '</span><br/>
                            </div>
                        ';
                    }
                }
            }
        }
    }
    public function carregaObjetivosEstrategicosPE($id){
        global $db;
        if ($id > 0) {
            $sql = "select obeid from pde.ae_objetivoestrategicoxmetappa where mppaid =" . $id;
            $dados = $db->carregarColuna($sql, 'obeid');
            $objetivos = implode(',', $dados);
            if ($objetivos) {
                $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($objetivos) order by obeordem";
                $rs = $db->carregar($sql);
                if ($rs) {
                    foreach ($rs as $data) {
                        echo '
                            <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 amarelo clickObjetivoEstrategico" style="padding: 0 0 20 0;">
                                <span class="textoDescricao">' . $data['nome'] . '</span><br/>
                            </div>
                        ';
                    }
                }
            }
        }
    }
    public function carregaMetasPNEPPA($id){
        global $db;
        if ($id > 0) {
            $sql = "select mpneid from pde.ae_metapnexppa where mppaid = {$id}";
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
                                            --and m.mpneid <> 99999999
                                            order by m.mpneordem";
                $rs = $db->carregar($sql);
                if ($rs) {
                    foreach ($rs as $data) {
                        echo '
                            <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 verde clickMetasPNE" style="padding: 0 0 20 0;">
                                <span class="textoDescricao">' . $data['nome'] . '</span><br/>
                                        </div>
                        ';
                    }
                }
            }
        }
    }
    public function carregaAcoesPEPPA($id){
        global $db;
        if ($id > 0) {
            $sql = "select acaid from pde.ae_acaoxmetappa where mppaid = {$id}";
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
                            <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['acaid'] . '"  style="padding: 0 0 20 0;">
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
    }
    public function carregaOrcamentosPPA($id){
        global $db;
        if ($id > 0) {
            $sql = "select acaid from pde.ae_acaoxmetappa where mppaid =" . $id;
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
                            <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');" style="padding: 0 0 20 0;">
                                                            <span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                            </div>
                        ';
                    }
                }
            }
        }
    }
    public function carregaProjetosPPA($id){
        global $db;
        if ($id > 0) {
            $sql = "SELECT DISTINCT acs.solid
                    FROM pto.metasolucao acs
                    INNER JOIN pde.ae_metapnexppa app ON app.mpneid = acs.mpneid
                    WHERE app.mppaid  = " . $id;
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
                            <div id="' . $data['temid'] . '_' . $data['codigo'] . '" class="tile1 tile4 marromClaro clickProjetos" style="padding: 0 0 20 0;">
                                <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
                                <span class="textoDescricao" >' . $data['nome'] . '</span><br/>
                                        </div>
                        ';
                    }
                }
            }
        }
    }
    public function montaGridOBJ($id){
        global $db;
        $arIds = explode('_', $id);
        ?>
        <div class="wrapper wrapper-content animated fadeIn">
		<div class="row">
                    <div class="col-md-6">
                        <div id="div_roll_pagina">
                            <?php
                            $sql = "SELECT
                                        ini.iniid,
                                        ini.ininome,
                                        ini.sitid AS situacao
                                    FROM pde.ae_iniciativa ini
                                    INNER JOIN pde.ae_objetivoestrategicoxiniciativa ins ON ins.iniid = ini.iniid
                                    WHERE ins.obeid = ".$arIds[1]. "
                                    ORDER BY ini.iniordem";
                            $rs = $db->carregar($sql);
                            ?>
                            <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                <thead>
                                    <tr>
                                        <td bgcolor="" width="20%" class="tituloInterno titulo">Iniciativas do Planejamento Estratégico</td>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr>
                                        <td>
                                            <?php
                                            if ($rs) {
                                                ?>
                                                <table class="textoDescricao" width="100%" border="1">
                                                    <tr>
                                                        <th width="80%">Iniciativa</th>
                                                        <th width="20%">Status</th>
                                                    </tr>
                                                <?php
                                                foreach($rs as $data){ ?>
                                                    <tr>
                                                        <td><?php echo $data['ininome']; ?></td>
                                                        <td class="center link situacaoandamento" align="center" onclick="situacaoandamento('<?=$data['iniid']?>');"><?php echo getImgSituacao($data['situacao']); ?></td>
                                                    </tr>
                                                <?php } ?>
                                                </table>
                                            <?php
                                            } 
                                            ?>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </div>
                    </div>
                
                    <div class="col-md-6">
                        <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                            <thead>
                                <tr>
                                    <td bgcolor="" width="20%" class="tituloInterno titulo">Metas PNE</td>
                                    <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Metas / Iniciativas PPA</td>
                                    <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Ações Estratégicas</td>
                                    <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Orçamentos</td>
                                    <!--td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Projetos</td-->
                                </tr>
                            </thead>
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
                                                                                        --and m.mpneid <> 99999999
                                                                                        order by m.mpneordem";
                                        $rs = $db->carregar($sql);

                                        if ($rs)
                                        {
                                            foreach($rs as $data)
                                            {
                                                echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE" style="padding: 0 0 20 0;">
                                                                                                  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                                            }
                                        }
                                    }
                                    ?>
                                </td>
                                <td id="div_objetivo_ppa" align="left" valign="top" style="padding-right:4px;">
                                    <?php
                                    //$sql = "select mppaid from pde.ae_objetivoestrategicoxmetappa where obeid =".$arIds[1];
                                    $sql = "SELECT mppaid
                                            FROM pde.ae_objetivoestrategicoxmetapne obe
                                            INNER JOIN pde.ae_metapnexppa met ON met.mpneid = obe.mpneid
                                            WHERE obe.obeid = ".$arIds[1];
                                    $dados = $db->carregarColuna($sql,'mppaid');
                                    $metas = implode(',',$dados);
                                    if ($metas){
                                        $sql = "select mppaid as codigo, COALESCE(mppanomeresumido,mppanome) AS nome, mppanome as nomecompleto, temid, o.objid, mppatipo
                                                from pde.ae_metappa m
                                                                            inner join pde.ae_objetivoppa o on m.objid = o.objid
                                                                            where mppaid in ($metas)
                                                                            order by mppatipo DESC, mppaordem";
                                        $rs = $db->carregar($sql);
                                        if ($rs) {
                                            foreach($rs as $data)
                                            {
                                                echo '<div id="'.$data['temid'].'_'.$data['objid'].'_'.$data['codigo'].'_'.$data['mppatipo'].'" class="tile1 tile4 azul clickMetasPPA" style="padding: 0 0 20 0;">
                                                                                                  <span class="textoDescricao" title="'.$data['nomecompleto'].'">'.$data['nome'].'</span><br/></div>';
                                            }
                                        }
                                    }
                                    ?>
                                </td>
                                <td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;">
                                    <?php
                                    //$sql = "select acaid from pde.ae_objetivoestrategicoxacao where obeid =".$arIds[1];
                                    $sql = "SELECT acaid
                                            FROM pde.ae_objetivoestrategicoxmetapne obe
                                            INNER JOIN pde.ae_acaoxmetapne met ON met.mpneid = obe.mpneid
                                            WHERE obe.obeid = ".$arIds[1];
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
                                                        <div class="tile1 tile4 laranja clickAcoesEstrategicas" id="' . $data['temid'] . '_' . $data['codigo'] . '"  style="padding: 0 0 20 0;">
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
                                    //$sql = "select acaid from pde.ae_objetivoestrategicoxacao where obeid =".$arIds[1];
                                    $sql = "SELECT acaid
                                            FROM pde.ae_objetivoestrategicoxmetapne obe
                                            INNER JOIN pde.ae_acaoxmetapne met ON met.mpneid = obe.mpneid
                                            WHERE obe.obeid = ".$arIds[1];
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
                                                    <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');" style="padding: 0 0 20 0;">
                                                        <span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                                                    </div>
                                                ';
                                            }
                                        }
                                    }
                                    ?>
                                </td>
                                <!--td id="div_projetos" align="left" valign="top" style="padding-right:4px;">
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
                                                echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 marromClaro clickProjetos" style="padding: 0 0 20 0;">
                                                            <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
                                                                                                    <span class="textoDescricao">'.$data['nome'].'</span><br/>
                                                                                                  </div>';
                                            }
                                        }
                                    }
                                    ?>
                                </td-->
                            </tr>
                        </table>
                    </div>
            </div>
        </div>
    <?php
    }
    public function montaGridDetalheAcaoAE($id){
            global $db;
            $arIds = explode('_', $id);
            $sql = "select aca.acaid, aca.acadetalhe, aca.indidprincipal, coc.coclink
                            from painel.acao aca
                            left join pde.cockpit coc ON coc.cocid = aca.cocid
                            where aca.acaid = {$arIds[1]}";
            $rs = $db->carregar($sql);
            if ($rs){
                echo($rs[0]['coclink'] ? '<a href="'.$rs[0]['coclink'].'" class="linkPopupPainel" target="_blank" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>':'');
                echo($rs[0]['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid='.$rs[0]['indidprincipal'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;':'');
                echo('<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid='.$rs[0]['acaid'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;'.$rs[0]['acadetalhe']);
            }
    }
    public function montaMetasAE($id){ 
        global $db;
        $arIds = explode('_', $id);
        $sql = "select mpneid from pde.ae_acaoxmetapne where acaid =".$arIds[1];
        $dados = $db->carregarColuna($sql,'mpneid');
        $metas = implode(',',$dados);

        if ($metas){
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
                                --and m.mpneid <> 99999999
                                order by m.mpneordem";
                $rs = $db->carregar($sql);

                if ($rs)
                {
                        foreach($rs as $data)
                        {
                                echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE" style="padding: 0 0 20 0;">
                                          <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                        }
                }
        }
    }
    public function montaPPAAE($id){
        global $db;
        $arIds = explode('_', $id);
        $sql = "select mppaid from pde.ae_acaoxmetappa where acaid =".$arIds[1];
        $dados = $db->carregarColuna($sql,'mppaid');
        $metas = implode(',',$dados);

        if ($metas){
            $sql = "select mppaid as codigo, COALESCE(mppanomeresumido,mppanome) AS nome, mppanome as nomecompleto, temid, o.objid, mppatipo from pde.ae_metappa m
                                        inner join pde.ae_objetivoppa o on m.objid = o.objid where mppaid in ($metas) order by mppatipo DESC, mppaordem";
            $rs = $db->carregar($sql);

            if ($rs)
            {
                foreach($rs as $data)
                {
                    echo '<div id="'.$data['temid'].'_'.$data['objid'].'_'.$data['codigo'].'_'.$data['mppatipo'].'" class="tile1 tile4 azul clickMetasPPA" style="padding: 0 0 20 0;">
                                                                  <span class="textoDescricao" title="'.$data['nomecompleto'].'">'.$data['nome'].'</span><br/></div>';
                }
            }
        }
    }
    public function montaOEAE($id){
        global $db;
        $arIds = explode('_', $id);
        $sql = "select obeid from pde.ae_objetivoestrategicoxacao where acaid =".$arIds[1];
        $dados = $db->carregarColuna($sql,'obeid');
        $objetivos = implode(',',$dados);

        if ($objetivos){
            $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($objetivos) order by obeordem";
            $rs = $db->carregar($sql);

            if ($rs)
            {
                foreach($rs as $data)
                {
                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 amarelo clickObjetivoEstrategico" style="padding: 0 0 20 0;">
                                                                  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                }
            }
        }
    }
    public function montaOrcamentosAE($id){
        global $db;
        $arIds = explode('_', $id);
        $sql = "SELECT vacid, vaetituloorcamentario AS nome
                        FROM planacomorc.vinculacaoacaoestrategica
                        WHERE acaid = ".$arIds[1]. "
                        ORDER BY nome";
        $rs = $db->carregar($sql);
        if ($rs) {
                foreach ($rs as $data) {
                        echo '
                                <div class="tile1 tile4 roxo" onclick="abreOrcamento('.$data['vacid'].');" style="padding: 0 0 20 0;">
                                        <span class="textoDescricao" title="' . $data['descricao'] . '">' . $data['nome'] . '</span><br/>
                                </div>
                        ';
                }
        }
    }
    public function montaProjetosAE($id){
        global $db;
        $arIds = explode('_', $id);
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
                   echo '<div id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 marromClaro clickProjetos" style="padding: 0 0 20 0;">
                           <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
                                                                   <span class="textoDescricao">'.$data['nome'].'</span><br/>
                                                                 </div>';
               }
           }
       }   
    }
    public function carregarRollAE($temid){
        global $db;
        $area = "ae";
        $sql = "select acaid as codigo, acadsc as descricao from painel.acao
                                    where acastatus = 'A' AND temid = {$temid}  order by acadsc ";
            $rs2 = $db->carregar($sql);
            $subTitulo = "Ações";
?>            
            <div class="wrapper wrapper-content animated fadeIn">
		<div class="row">
                    <div class="col-md-6">
                        <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                            <tr>
                                <td style='font-size: 20px; text-align:center; font-weight:bold; background-color: #E4D779;' class='tituloAno' >
                                    <?php echo $subTitulo;?>
                                </td>
                            </tr>
<?php            
                        if ($rs2) {
?>

                            <tr>
                                <td>
                                    <div id="div_lnk_objetivos_metas">
                                        <div class="wrapper wrapper-content animated fadeIn"> 
                                            <div class="row">
                                                <div style="margin: 0 auto; padding: 0; height: 312px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
<?php
                                                    foreach ($rs2 as $dados) {

                                                                    echo '<div title="'.$dados['descricao'].'"id="' . $area . '_' . $dados['codigo'] . '"  class="tile1 tile4  objetivos_metas_ae" style="cursor:pointer;border-bottom: 1px #CCC solid;padding:5px 0 20 0;"> ';
                                                                    echo $dados['descricao'];
                                                                    echo '</div>';

                                                    }
?>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </td>
                            </tr>
                        </table>
                    </div>
                    <div class="col-md-6">
                        <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                            <tr>
                                    <td  bgcolor="#4682B4" class="titulo">Detalhe da Ação</td>
                            </tr>
                            <tr>
                                    <td colspan="3">
                                            <div id="div_roll_pagina">
                                                <div class="tile1  tile4 azulClaro acoesae" style="cursor:default;padding: 0 0 20 0;"><br/><br/>
                                                </div>
                                            </div>
                                    </td>
                            </tr>
                            <tr>
                                <td>
                                    <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                        <thead>
                                            <tr>
                                                    <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Metas PNE</td>
                                                    <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Metas / Iniciativas PPA</td>
                                                    <!--<td bgcolor="#4682B4" width="20%" class="tituloInterno">Objetivos Estratégicos e Desafios</span></td>-->
                                                    <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Objetivos Estratégicos</td>
                                                    <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Orçamentos</td>
                                                    <!--td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Projetos</td-->
                                            </tr>
                                        </thead>
                                            <tr>
                                                <td colspan="4">
                                                    <div style="margin: 0 auto; padding: 0; height: 200px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
                                                        <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                                            <tr>
                                                                <td id="div_metas_pne" align="left" valign="top" width="20%" style="padding-right:4px;">
                                                                </td>
                                                                <td id="div_objetivo_ppa" align="left" valign="top" width="21%" style="padding-right:4px;">
                                                                </td>
                                                                <td id="div_acoes_metas" align="left" valign="top" width="21%" style="padding-right:4px;">
                                                                </td>
                                                                <td id="div_orcamentos" align="left" valign="top" width="18%" style="padding-right:4px;">
                                                                </td>
                                                                <!--td id="div_projetos" align="left" valign="top" style="padding-right:4px;">
                                                                </td-->
                                                            </tr>
                                                        </table>
                                                    </div>
                                                </td>
                                            </tr>
                                    </table>
                                </td>
                                </tr>
                            </table>
                    </div>
                </div>
            </div>
<?php                    
            }
    }
    public function montaGridOrc($id){
        global $db;
        $arIds = explode('_', $id);
        $sql = "SELECT sba.descricao FROM planacomorc.subacao sba WHERE sba.codigo = '{$arIds[1]}'";
        $rs = $db->carregar($sql);
        ?>

        <div class="col-md-6">
                    <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                        <tr>
                                <td  bgcolor="#4682B4"><span class="titulo">Detalhe do Orçamento</span></td>
                        </tr>
                        <tr>
                                <td colspan="3">
                                        <div id="div_roll_pagina">
                                                <div class="tile1  tile4 azulClaro orcamentos" style="cursor:default;padding: 0 0 20 0;">
                                                <?php
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
                            <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                <thead>
                                <tr>
                                        <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas PNE</span></td>
                                        <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Metas / Iniciativas PPA</span></td>
                                        <!--<td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos e Desafios</span></td>-->
                                        <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Objetivos Estratégicos</span></td>
                                        <td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Ações Estratégicas</span></td>
                                        <!--td bgcolor="#4682B4" width="20%"><span class="tituloInterno">Projetos</span></td-->
                                </tr>
                                </thead>
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
                                                                        --and m.mpneid <> 99999999
                                                                        order by m.mpneordem";
                                                        $rs = $db->carregar($sql);

                                                        if ($rs)
                                                        {
                                                                foreach($rs as $data)
                                                                {
                                                                        echo '<div  id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE" style="padding: 0 0 20 0;">
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
                            $sql = "select mppaid as codigo, COALESCE(mppanomeresumido,mppanome) AS nome, mppanome as nomecompleto, temid, o.objid, mppatipo from pde.ae_metappa
                                                                        m  inner join pde.ae_objetivoppa o on m.objid = o.objid where mppaid in ($metas) order by mppatipo DESC, mppaordem";
                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['objid'].'_'.$data['codigo'].'_'.$data['mppatipo'].'" class="tile1 tile4 azul clickMetasPPA" style="padding: 0 0 20 0;">
                                                                                  <span class="textoDescricao" title="'.$data['nomecompleto'].'">'.$data['nome'].'</span><br/></div>';
                                }
                            }
                        }
                        ?>
                    </td>
                                        <td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;">
                                                <?php
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
                            $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($desafios) order by obeordem";

                            $rs = $db->carregar($sql);

                            if ($rs)
                            {
                                foreach($rs as $data)
                                {
                                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'"  class="tile1 tile4 amarelo clickObjetivoEstrategico" style="padding: 0 0 20 0;">
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
                                                                        echo ('<div id="'.$data['temid'].'_'.$data['acaid'].'" class="tile1 tile4 laranja clickAcoesEstrategicas" style="padding: 0 0 20 0;">');
                                                                        echo ($data['coclink'] ? '<a href="'.$data['coclink'].'" class="linkPopupPainel" target="_blank" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>':'');
                                                                        echo ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid='.$data['indidprincipal'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '');
                                                                        echo ('<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid='.$data['acaid'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;' . $data['acadsc']);
                                                                        echo '</div>';
                                                                }
                                                        }
                                                }
                                                ?>
                                        </td>
                    <!--td id="div_projetos" align="left" valign="top" style="padding-right:4px;">
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
                                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'"  class="tile1 tile4 marromClaro clickProjetos" style="padding: 0 0 20 0;">
                                                <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
                                                                                        <span class="textoDescricao">'.$data['nome'].'</span><br/>
                                                                                  </div>';
                                }
                            }
                        }
                        ?>
                    </td-->
                                </tr>
                        </table>
                        </td>
                        </tr>
                </table>
        </div>
        <?php
    }
    public function carregarRollORC($temid){
        
        global $db;
        $sql = "SELECT DISTINCT sba.codigo AS codigo, sba.codigo || ' - ' || sba.titulo AS descricao
					FROM planacomorc.vinculacaoestrategicasubacoes ves
					INNER JOIN planacomorc.subacao sba ON sba.codigo = ves.sbacod
					INNER JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vae.vaeid = ves.vaeid
					INNER JOIN planacomorc.vinculacaoacaoestrategica vac ON vac.vacid = vae.vacid
					INNER JOIN painel.acao aca ON aca.acaid = vac.acaid
					WHERE aca.temid = {$temid}
					ORDER BY descricao";
            $subTitulo = "Subações Orçamentárias";
            $rs2 = $db->carregar($sql);
 ?>
            <div class="wrapper wrapper-content animated fadeIn">
		<div class="row">
                    <div class="col-md-6">
                        <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                            <tr>
                                <td style='font-size: 20px; text-align:center; font-weight:bold; background-color: #E4D779;' class='tituloAno' >
                                    <?php echo $subTitulo;?>
                                </td>
                            </tr>
<?php            
                        if ($rs2) {
?>

                            <tr>
                                <td>
                                    <div id="div_lnk_objetivos_metas">
                                        <div class="wrapper wrapper-content animated fadeIn"> 
                                            <div class="row">
                                                <div style="margin: 0 auto; padding: 0; height: 368px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
<?php
                                                    foreach ($rs2 as $dados) {

                                                                    echo '<div title="'.$dados['descricao'].'"id="' . $area . '_' . $dados['codigo'] . '"  class="tile1 tile4  objetivos_metas_orc" style="cursor:pointer;border-bottom: 1px #CCC solid;padding:5px 0 20 0;"> ';
                                                                    echo $dados['descricao'];
                                                                    echo '</div>';

                                                    }
?>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </td>
                            </tr>
<?php                            
                            }
?>                            
                        </table>
                    </div>
                    <div class="col-md-6">
                        <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                <tr>
                                        <td  bgcolor="#4682B4" class="titulo">Detalhe do Orçamento</td>
                                </tr>
                                <tr>
                                        <td colspan="3">
                                                <div id="div_roll_pagina">
                                                    <div class="tile1  tile4 azulClaro acoesae" style="cursor:default;padding: 0 0 20 0;"><br/><br/>
                                                    </div>
                                                </div>
                                        </td>
                                </tr>
                                <tr>
                                    <td>
                                        <table border='2' align='center' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                            <thead>
                                                <tr>
                                                        <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Metas PNE</td>
                                                        <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Metas / Iniciativas PPA</td>
                                                        <!--<td bgcolor="#4682B4" width="20%" class="tituloInterno">Objetivos Estratégicos e Desafios</span></td>-->
                                                        <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Objetivos Estratégicos</td>
                                                        <td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Ações Estratégicas</td>
                                                        <!--td bgcolor="#4682B4" width="20%" class="tituloInterno titulo">Projetos</td-->
                                                </tr>
                                            </thead>
                                                <tr>
                                                    <td colspan="4">
                                                        <div style="margin: 0 auto; padding: 0; height: 220px; width: 100%; border: none; overflow-x: auto; overflow-y: scroll;">
                                                            <table border='2'  align='center' width='98%' cellspacing='4' cellpadding='5' class='table table-striped table-bordered table-hover'>
                                                                <tr>
                                                                    <td id="div_metas_pne" align="left" valign="top" width="20%" style="padding-right:4px;">
                                                                    </td>
                                                                    <td id="div_objetivo_ppa" align="left" valign="top" width="21%" style="padding-right:4px;">
                                                                    </td>
                                                                    <td id="div_acoes_metas" align="left" valign="top" width="21%" style="padding-right:4px;">
                                                                    </td>
                                                                    <td id="div_orcamentos" align="left" valign="top" width="18%" style="padding-right:4px;">
                                                                    </td>
                                                                    <!--td id="div_projetos" align="left" valign="top" style="padding-right:4px;">
                                                                    </td-->
                                                                </tr>
                                                            </table>
                                                        </div>
                                                    </td>
                                                </tr>
                                        </table>
                                    </td>
                                    </tr>
                                </table>
                    </div>                
                </div>
<?php
                            
    }
    public function montaGridDetalheAcaoORC($id){
        global $db;
        $arIds = explode('_', $id);
        $sql = "SELECT sba.descricao FROM planacomorc.subacao sba WHERE sba.codigo = '{$arIds[1]}'";
        $rs = $db->carregar($sql);
        if ($rs){
                 echo $rs[0]['descricao'];
        }
    }
    public function montaMetasORC($id){
        global $db;
        $arIds = explode('_', $id);
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
                                --and m.mpneid <> 99999999
                                order by m.mpneordem";
                $rs = $db->carregar($sql);

                if ($rs)
                {
                        foreach($rs as $data)
                        {
                                echo '<div  id="'.$data['temid'].'_'.$data['codigo'].'" class="tile1 tile4 verde clickMetasPNE" style="padding: 0 0 20 0;">
                                          <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                        }
                }
        }
    }
    public function montaPPAORC($id){
        global $db;
        $arIds = explode('_', $id);
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
            $sql = "select mppaid as codigo, COALESCE(mppanomeresumido,mppanome) AS nome, mppanome as nomecompleto, temid, o.objid, mppatipo from pde.ae_metappa
                                                        m  inner join pde.ae_objetivoppa o on m.objid = o.objid where mppaid in ($metas) order by mppatipo DESC, mppaordem";
            $rs = $db->carregar($sql);

            if ($rs)
            {
                foreach($rs as $data)
                {
                    echo '<div id="'.$data['temid'].'_'.$data['objid'].'_'.$data['codigo'].'_'.$data['mppatipo'].'" class="tile1 tile4 azul clickMetasPPA" style="padding: 0 0 20 0;">
                                                                  <span class="textoDescricao" title="'.$data['nomecompleto'].'">'.$data['nome'].'</span><br/></div>';
                }
            }
        }
    }
    public function montaOEORC($id){
        global $db;
        $arIds = explode('_', $id);
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
            $sql = "select obeid as codigo, obenome as nome, temid from pde.ae_objetivoestrategico where obeid in ($desafios) order by obeordem";

            $rs = $db->carregar($sql);

            if ($rs)
            {
                foreach($rs as $data)
                {
                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'"  class="tile1 tile4 amarelo clickObjetivoEstrategico" style="padding: 0 0 20 0;">
                                                                  <span class="textoDescricao">'.$data['nome'].'</span><br/></div>';
                }
            }
        }
    }
    public function montaOrcamentosORC($id){
        global $db;
        $arIds = explode('_', $id);
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
                                echo ('<div id="'.$data['temid'].'_'.$data['acaid'].'" class="tile1 tile4 laranja clickAcoesEstrategicas" style="padding: 0 0 20 0;">');
                                echo ($data['coclink'] ? '<a href="'.$data['coclink'].'" class="linkPopupPainel" target="_blank" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>':'');
                                echo ($data['indidprincipal'] ? '<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&abreMapa=1&cockpit=1&indid='.$data['indidprincipal'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '');
                                echo ('<a href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&acaid='.$data['acaid'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;' . $data['acadsc']);
                                echo '</div>';
                        }
                }
        }
    }
    public function montaProjetosORC($id){
        global $db;
        $arIds = explode('_', $id);
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
                    echo '<div id="'.$data['temid'].'_'.$data['codigo'].'"  class="tile1 tile4 marromClaro clickProjetos" style="padding: 0 0 20 0;">
                                <a href="/pto/pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid='. $data['codigo'] .'" target="_blank"><img src="../imagens/icones/icons/Preview.png" height="30px" width="30px" title="Detalhar Projeto"></a>&nbsp;
                                                                        <span class="textoDescricao">'.$data['nome'].'</span><br/>
                                                                  </div>';
                }
            }
        }
    }
}

//========Iniciativas do Planejamento Estratégico=============
if ($_REQUEST['tbs'] == 's' && $_REQUEST['iniid'] > 0) {
    ob_clean();
    header("Content-Type: text/html; charset=ISO-8859-1");
    $sql = "SELECT to_char(stidata, 'DD/MM/YYYY'), stidsc
            FROM pde.ae_situacaoiniciativa
            WHERE iniid = {$_REQUEST['iniid']}
            ORDER BY stidata DESC";
    $cabecalho = array('Data', 'Situação');

    $db->monta_lista_simples($sql, $cabecalho, 200, 20, '', '', '');
    die();
}

function getImgSituacao($situacao)
{
    switch ($situacao) {
        case 4;
            return '<i style="font-size: 22px;text-align:center;color:#67DB56;cursor: pointer;" class="glyphicon glyphicon-ok-sign" height="15px" width="15px" title="Parcialmente atendida"></i>';
            break;
        case 3;
            return '<i style="font-size: 22px;text-align:center;color:#67DB56;cursor: pointer;" class="glyphicon glyphicon-ok" height="15px" width="15px" title="Atendida"></i>';
            break;
        case 2;
            return '<i style="font-size: 22px;text-align:center;color:#FFB400;cursor: pointer;" class="glyphicon glyphicon-pencil" height="15px" width="15px" title="Em atendimento"></i>';
            break;
        case 1;
            return '<i style="font-size: 22px;text-align:center;color:#f00;cursor: pointer;" class="glyphicon glyphicon-remove-circle" height="15px" width="15px" title="Não atendida"></i>';
            break;
    }
}
?>
<script>
function situacaoandamento(iniid) {
    $('#sitand_id').load('estrategico.php?modulo=principal/alinhamento_estrategico/alinhamento_estrategico&layout=novo&acao=A&tbs=s&iniid=' + iniid);
    $('#myModalSit').modal();
}
</script>
<div class="modal fade" id="myModalSit" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Situação</h4>
            </div>
            <div class="modal-body">
                <div id="sitand_id">

                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">fechar</button>
            </div>
        </div>
    </div>
</div>
<?php
//============================================================
?>
