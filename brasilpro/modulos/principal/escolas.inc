<?php
//
// $Id$
//


cte_verificaSessao();

if ($_REQUEST['opt']) {
    if ($_REQUEST['opt'] == 'carregarArvorePPP') {
        $sql2   = "select * from cte.itensppp where ipppai = %d ORDER BY ippordem";
        $sql3   = "select count(*) from cte.conteudoppp where ipppai = %d";
        $sql4   = "select count(*) from cte.conteudoppp where ippid = %d AND entid = %d";

        $entid  = $_REQUEST['entid'];
        $ippid1 = $_REQUEST['ippid'];

        $arr2   = $db->carregar(sprintf($sql2, $ippid1));
        $arr2   = is_array($arr2) ? $arr2 : array();

        foreach ($arr2 as $item2) {
            $ippid2       = $item2["ippid"];
            $ipppai2      = $item2["ipppai"];
            $ordem2       = $item2["ordem"];
            $ipptitulo2   = $item2["ipptitulo"];
            $ippconteudo2 = $item2["ippconteudo"];
            $ipptr2       = $item2["ipptiporesposta"];

            $res = $db->pegaUm(sprintf($sql4, $ippid2, $entid));

            if ($res == 1)
                $img = ", '', '', '../imagens/check_p.gif'";
            else
                $img  = '';


            if ($ippconteudo2 == 1) {
                echo "arvore.add('ipp1_" , $entid , $ippid2 , "', 'ipp_" , $entid , $ippid1 , "', '" , $ipptitulo2 , "', 'javascript:abreComboPPP(" , $ippid2 , ", " , $entid , ", $ipptr2);'" , $img , ");";
            } else {
                echo "arvore.add('ipp1_" , $entid , $ippid2 , "', 'ipp_" , $entid , $ippid1 , "', '" , $ipptitulo2 , "', ''" , $img , ");";
            }

            $arr3 = $db->carregar(sprintf($sql2, $ippid2));
            $arr3 = is_array($arr3) ? $arr3 : array();

            foreach ($arr3 as $item3) {
                $ippid3       = $item3["ippid"];
                $ipppai3      = $item3["ipppai"];
                $ordem3       = $item3["ordem"];
                $ipptitulo3   = $item3["ipptitulo"];
                $ippconteudo3 = $item3["ippconteudo"];
                $ipptr3       = $item3["ipptiporesposta"];

                $res = $db->pegaUm(sprintf($sql4, $ippid3, $entid));
                if ($res == 1)
                    $img = ", '', '', '../imagens/check_p.gif'";
                else
                    $img  = '';//", '', '', '../imagens/atencao.png'";

                if ($ippconteudo3 == 1) {
                    echo "arvore.add('ipp2_" , $entid , $ippid3 , "', 'ipp1_" , $entid , $ippid2 , "', '" , $ipptitulo3 , "', 'javascript:abreComboPPP(" , $ippid3 , ", " , $entid , ", $ipptr3);'" , $img , ");";
                } else {
                    echo "arvore.add('ipp2_" , $entid , $ippid3 , "', 'ipp1_" , $entid , $ippid2 , "', '" , $ipptitulo3 , "', 'javascript:void(0)'" , $img , ");";
                }
            }
        }

        echo 'elemento = document.getElementById("_arvore");'
            .'elemento.innerHTML = arvore;'
            .'$("aguarde").style.visibility = "hidden";'
            .'$("aguarde").style.display = "none";';

        die();
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';
cte_montaTitulo( $titulo_modulo, '' );

$inuid = (integer) $_SESSION["inuid"];
$itrid = cte_pegarItrid( $inuid );
$sql = "
    select
        iue.entid,
        '[' || coalesce(e.entcodent, '') || '] ' || e.entnome as entnome,
        mu.muncod,
        mu.mundescricao
    from cte.instrumentounidadeescola iue
        inner join entidade.entidade e on
            e.entid = iue.entid
        INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
        inner join entidade.endereco ende on
            ende.entid = e.entid
        inner join territorios.municipio mu on
            mu.muncod = ende.muncod
        left join entidade.entidadedetalhe edd on
            e.entid = edd.entid
            and
            (
                entdreg_medio_prof      = '1' or
                entdreg_medio_medio     = '1' or
                entdreg_medio_normal    = '1' or
                entdreg_medio_integrado = '1'
            )
    where
        iue.inuid = " . $inuid;

if ($itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL) {
    $sql .= " AND e.tpcid = 1 AND fe.funid = 3 ";
} else {
    $sql .= " AND e.tpcid = 3 AND fe.funid = 3 ";
}

$sql .= "
    group by
        iue.entid,
        e.entcodent,
        e.entnome,
        mu.muncod,
        mu.mundescricao
    order by
        mu.mundescricao ASC
";

$escolas = $db->carregar( $sql );
$escolas = $escolas ? $escolas : array();

?>
<script type="text/javascript" src="../includes/dtree/dtree.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <colgroup>
        <col/>
    </colgroup>
    <tbody>
        <tr>
            <td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
                <div id="bloco" style="overflow: hidden;">
                    <p>
                        <a href="javascript: arvore.openAll();">Abrir Todos</a>
                        &nbsp;|&nbsp;
                        <a href="javascript: arvore.closeAll();">Fechar Todos</a>
                    </p>
                    <div id="_arvore"></div>
                </div>
            </td>
        </tr>
    </tbody>
</table>



<script type="text/javascript"><!--
    var arvorePPPCarregada = new Array();
    /**
     * 
     */
    function carregarArvorePPP(ev, entid, ippid)
    {
        var no = entid + ippid;
        if (arvorePPPCarregada[no])
            return false;

        $('aguarde').style.visibility = '';
        $('aguarde').style.display    = 'block';

        var req = new Ajax.Request('?modulo=principal/escolas&acao=A', {
                                   parameters: '&opt=carregarArvorePPP&entid=' + entid + '&ippid=' + ippid,
                                   method: 'post',
                                   onComplete: function(res)
                                   {
                                       arvorePPPCarregada[no] = true;
                                       eval(res.responseText);
                                       //alert(res.responseText);
                                   }
        });

        return false;
    }

    function abreComboPPP(ippid, entid, ipptiporesposta)
    {
        return windowOpen( '?modulo=principal/escolappp&acao=A&entid=' + entid + '&ippid=' + ippid + '&ipptiporesposta=' + ipptiporesposta, 'escolappp','height=550,width=550,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
    }

    function alterarSubacao( sbaid )
    {
        return windowOpen( "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
    }
    
    function emitirRelatorioPPP(entid)
    {
        return windowOpen( '?modulo=principal/emissaorelatorioppp&acao=A&entid=' + entid, 'emissaorelatorioppp','height=550,width=550,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
    }

    function escolherEscolas()
    {
        return windowOpen( '?modulo=principal/escolasescolha&acao=A','blank','height=400,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }

    function manterEscola(entid)
    {
        return window.location.href = '?modulo=principal/manterescolaatendida&acao=A&entid=' + entid;
    }

    function incluirEscola(entid)
    {
        if (entid)
            return windowOpen( '?modulo=principal/cadastrarescola&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=800,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
        else
            return windowOpen( '?modulo=principal/cadastrarescola&acao=A','blank','height=800,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }

    function voltar()
    {
        window.location.href = '?modulo=principal/estrutura_avaliacao&acao=A';
    }

    var arvore = new dTree( 'arvore' );
    arvore.config.folderLinks = true;
    arvore.config.useIcons = true;
    arvore.config.useCookies = true; 

    arvore.add( 1, -1, "Escolas Atendidas", 'javascript:void(0);' );

    <?php
        $municipios_adicionados = array();
        foreach ( $escolas as $escola ) {
            $entid        = (integer) $escola["entid"];
            $entnome      = $escola["entnome"];
            $muncod       = $escola["muncod"];
            $mundescricao = $escola["mundescricao"];
            $codigo_pai   = 1;

            if ($itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL) {
			    $codigo_pai = "mu_" . $muncod;
                if (!in_array( $muncod, $municipios_adicionados ))
                    echo 'arvore.add(\'' , $codigo_pai , '\', 1, \'' , addslashes($mundescricao) , '\');';
            }

            array_push( $municipios_adicionados, $muncod );

            $nome   = '<a href=\\"?modulo=principal/escola&acao=A&entid=' . $entid . '\\">' . addslashes($entnome) . '</a>';
            $codigo = 'es_' . $entid;
		?>
		arvore.add('<?= $codigo ?>', '<?= $codigo_pai ?>', '<?php echo addslashes($entnome); ?>', 'javascript:manterEscola(<?= $entid ?>)');

        //arvore.add('ppp_<?= $entid ?>', 'es_<?= $entid ?>', 'PPP - Plano Político Pedagógico');

        <?php
        continue;

        //---------------------------------------------------------- Itens PPP
        $sql1  = "select * from cte.itensppp where ipppai is null ORDER BY ippordem";
        $sql2  = "select * from cte.itensppp where ipppai = %d ORDER BY ippordem";
        $sql3  = "select count(*) from cte.conteudoppp where ipppai = %d";
        $sql4  = "select count(*) from cte.conteudoppp where ippid = %d AND entid = %d";
        //                                                                  */

        $arr1  = $db->carregar($sql1);
        $arr1  = is_array($arr1) ? $arr1 : array();
        $count = 0;

        foreach ($arr1 as $item1) {
            $ippid1       = $item1["ippid"];
            $ipppai1      = $item1["ipppai"];
            $ordem1       = $item1["ippordem"];
            $ipptitulo1   = $item1["ipptitulo"];
            $ippconteudo1 = $item1["ippconteudo"];
            $ipptr1       = $item1["ipptiporesposta"];

            $arr2 = $db->carregar(sprintf($sql2, $ippid1));
            $res  = $db->pegaUm(sprintf($sql4, $ippid1, $entid));

            if ($res == 1)
                $img = ", '', '', '../imagens/check_p.gif'";
            else
                $img  = '';//", '', '', '../imagens/atencao.png'";

            if ($ippconteudo1 == 1) {
                echo "arvore.add('ipp_" , $entid , $ippid1 , "', 'ppp_" , $entid , "', '" , $ipptitulo1 , "', 'javascript:abreComboPPP(" , $ippid1 , ", " , $entid , ", \'ipp_" , $entid , $ippid1 , "\', $ipptr1);'" , $img , ");\n";
            } else {
                $img = ", '', '', '../includes/dtree/img/folder.gif'";
                echo "arvore.add('ipp_" , $entid , $ippid1 , "', 'ppp_" , $entid , "', '" , $ipptitulo1 , "', 'javascript:void(0);\" onclick=\"return carregarArvorePPP(event, " , $entid , ", " , $ippid1 , ");'" , $img , ");\n";
            }

            /*!@

            $arr2 = $db->carregar(sprintf($sql2, $ippid1));
            $arr2 = is_array($arr2) ? $arr2 : array();

            foreach ($arr2 as $item2) {
                $ippid2       = $item2["ippid"];
                $ipppai2      = $item2["ipppai"];
                $ordem2       = $item2["ordem"];
                $ipptitulo2   = $item2["ipptitulo"];
                $ippconteudo2 = $item2["ippconteudo"];
                $ipptr2       = $item2["ipptiporesposta"];

                $res = $db->pegaUm(sprintf($sql4, $ippid2, $entid));

                if ($res == 1)
                    $img = ", '', '', '../imagens/check_p.gif'";
                else
                    $img  = '';


                if ($ippconteudo2 == 1) {
                    echo "arvore.add('ipp1_" , $entid , $ippid2 , "', 'ipp_" , $entid , $ippid1 , "', '" , $ipptitulo2 , "', 'javascript:abreComboPPP(" , $ippid2 , ", " , $entid , ", $ipptr2);'" , $img , ");\n";
                } else {
                    echo "arvore.add('ipp1_" , $entid , $ippid2 , "', 'ipp_" , $entid , $ippid1 , "', '" , $ipptitulo2 , "', ''" , $img , ");\n";
                }

                $arr3 = $db->carregar(sprintf($sql2, $ippid2));
                $arr3 = is_array($arr3) ? $arr3 : array();

                foreach ($arr3 as $item3) {
                    $ippid3       = $item3["ippid"];
                    $ipppai3      = $item3["ipppai"];
                    $ordem3       = $item3["ordem"];
                    $ipptitulo3   = $item3["ipptitulo"];
                    $ippconteudo3 = $item3["ippconteudo"];
                    $ipptr3       = $item3["ipptiporesposta"];

                    $res = $db->pegaUm(sprintf($sql4, $ippid3, $entid));
                    if ($res == 1)
                        $img = ", '', '', '../imagens/check_p.gif'";
                    else
                        $img  = '';//", '', '', '../imagens/atencao.png'";

                    if ($ippconteudo3 == 1) {
                        echo "arvore.add('ipp2_" , $entid , $ippid3 , "', 'ipp1_" , $entid , $ippid2 , "', '" , $ipptitulo3 , "', 'javascript:abreComboPPP(" , $ippid3 , ", " , $entid , ", $ipptr3);'" , $img , ");\n";
                    } else {
                        echo "arvore.add('ipp2_" , $entid , $ippid3 , "', 'ipp1_" , $entid , $ippid2 , "', '" , $ipptitulo3 , "', 'javascript:void(0)'" , $img , ");\n";
                    }
                }
            }
            //                                                              */
        }

        //--------------------------------------------------------------------

        $sql = "
            select
                sa.sbaid,
                sa.sbadsc
            from cte.qtdfisicoano qfa
                inner join cte.subacaoindicador sa on
                    sa.sbaid = qfa.sbaid
                inner join cte.acaoindicador ac on
                    ac.aciid = sa.aciid
                inner join cte.pontuacao po on
                    po.ptoid = ac.ptoid
            where
                qfa.entid = " . $entid . " and
                po.ptostatus = 'A' and
                po.inuid = " . $inuid . "
            group by
                sa.sbaid,
                sa.sbadsc";   
        

        $subacaoes = $db->carregar( $sql );
        $subacaoes = $subacaoes ? $subacaoes : array();
        $codigo_subacao = "sa_pai_" . $entid;


        //echo 'arvore.add("relatorio", "' , $codigo , '", "Relatório PPP", "javascript:emitirRelatorioPPP(' , $entid , ');");
        ?>
        arvore.add( 'relatorio', '<?= $codigo ?>', "Relatório PPP", "javascript:emitirRelatorioPPP(<?php echo $entid ?>);" );
        arvore.add( '<?= $codigo_subacao ?>', '<?= $codigo ?>', "Sub-ações (<?= count( $subacaoes ) ?>)", 'javascript:void(0);' );
        <?php foreach ( $subacaoes as $subacao ): ?>
            arvore.add( 'sa_<?= $subacao["sbaid"] ?>', '<?= $codigo_subacao ?>', "<?= $subacao["sbadsc"] ?>", "javascript:alterarSubacao( '<?= $subacao["sbaid"] ?>' );" );
        <?php endforeach;
        }
?>

    elemento = document.getElementById( '_arvore' );
    elemento.innerHTML = arvore;
--></script>


<p align="center">
  <input type="button" name="incluirEscola" value="Cadastrar Escola" onclick="incluirEscola();" />&nbsp;&nbsp;&nbsp;
  <input type="button" name="AddRemove" value="Adicionar / Remover" onclick="escolherEscolas();" />&nbsp;&nbsp;&nbsp;
  <input type="button" name="Voltar" value="Voltar" align="right" onclick="voltar();" />
</p>

