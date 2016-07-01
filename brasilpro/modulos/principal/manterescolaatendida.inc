<?php
//
// $Id$
//


cte_verificaSessao();

if (!array_key_exists('entid', $_REQUEST)) {
    echo '<script type="text/javascript">'
        .'alert("O código da escola não foi informado.");'
        .'history.back();'
        .'</script>';
} else {
    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br />';
    cte_montaTitulo($titulo_modulo, '');
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
                        <a href="javascript: arvore.openAll();">Abrir Todos</a>&nbsp;|&nbsp;<a href="javascript: arvore.closeAll();">Fechar Todos</a>
                    </p>
                    <div id="_arvore"></div>
                </div>
            </td>
        </tr>
    </tbody>
</table>


<script type="text/javascript">
<!--
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

    function alterarEscola(entid)
    {
        return windowOpen( '?modulo=principal/cadastrarescola&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=800,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }

    function bloquearPPP()
    {
    	alert("Por favor, indique se a escola possui ou não a formação de EPT");
    }
    
    function alterarEPT(entid)
    {
        return windowOpen( '?modulo=principal/alterarformacaoept&acao=A&entid=' + entid, 'indicarept','height=550,width=630,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
    }

-->
</script>

<?php
    $entid = $_REQUEST['entid'];
    $inuid = (integer) $_SESSION["inuid"];
    $itrid = cte_pegarItrid( $inuid );
    
    $sql = 'SELECT ent.entnome, entd.entdoferta_ept FROM entidade.entidade AS ent
    		left JOIN entidade.entidadedetalhe AS entd ON entd.entcodent = ent.entcodent  
    		WHERE ent.entid = ' . $_REQUEST['entid'];
    $ent = (array) $db->carregar($sql);
    
    if(!$ent[0]) {
		$sql = 'SELECT ent.entnome, entd.entdoferta_ept FROM entidade.entidade AS ent
    			left JOIN entidade.entidadedetalhe AS entd ON entd.entid = ent.entid
    			WHERE ent.entid = ' . $_REQUEST['entid'];
	    $ent = (array) $db->carregar($sql);
	    
    }   
    
    if (count($ent) == 0) {
        echo '<script type="text/javascript">'
            .'alert("Escola não cadastrada.");'
            .'history.back();'
            .'</script>';
    }

    echo '
    <script type="text/javascript">
    <!--
        var arvore = new dTree("arvore");
        arvore.config.folderLinks = true;
        arvore.config.useIcons    = true;
        arvore.config.useCookies  = true;
        arvore.add(0, -1, \'Escolas Atendidas\', \'javascript:history.back()\');
        arvore.add(1,  0, \'' , addslashes($ent[0]['entnome']) , '\', \'javascript:alterarEscola(' ,$_REQUEST['entid'], ')\');
        arvore.add(2,  0, \'Indicar se há oferta de EPT (Educação profissional e Tecnológica)\', \'javascript:alterarEPT(', $_REQUEST['entid'] ,')\');';
    	
    	if($ent[0]['entdoferta_ept'] != '') {
        	echo 'arvore.add("ppp_' , $entid , '", 0, "PPP - Plano Político Pedagógico");' , "\n";
    	} else {
        	echo 'arvore.add(3, 0, "PPP - Plano Político Pedagógico", \'javascript:bloquearPPP();\',"PPP - Plano Político Pedagógico","",\'../includes/dtree/img/folder.gif\');', "\n";
    	}
        //------------------------------------------------------ Itens PPP
        $sql1  = "select * from cte.itensppp where ipppai is null ORDER BY ippordem";
        $sql2  = "select * from cte.itensppp where ipppai = %d ORDER BY ippordem";
        $sql3  = "select count(*) from cte.conteudoppp where ipppai = %d";
        $sql4  = "select count(*) from cte.conteudoppp where ippid = %d AND entid = %d";
        //                                                              */

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
               	echo "arvore.add('ipp_" , $entid , $ippid1 , "', 'ppp_" , $entid , "', '" , $ipptitulo1 , "', 'javascript:abreComboPPP(" , $ippid1 , ", " , $entid , ", $ipptr1);'" , $img , ");\n";
           	} else {
               	$img = ", '', '', '../includes/dtree/img/folder.gif'";
               	echo 'arvore.add("ipp_' , $entid , $ippid1 , '", "ppp_' , $entid , '", "' , $ipptitulo1 , '", ""' , $img , ");\n";
           	}


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

                    $res = (integer) $db->pegaUm(sprintf($sql4, $ippid3, $entid));
                    if ($res >= 1)
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

        //----------------------------------------------------------------
        $sql = "select sa.sbaid, sa.sbadsc, fe.frmdsc, ss.ssudescricao, ss.ssuid
				from cte.qtdfisicoano qfa 
					inner join cte.subacaoindicador sa on sa.sbaid = qfa.sbaid
					inner join cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid and sptano = qfaano
					inner join cte.statussubacao ss on ss.ssuid = spt.ssuid
					inner join cte.formaexecucao fe on fe.frmid = sa.frmid
					inner join cte.acaoindicador ac on ac.aciid = sa.aciid
					inner join cte.pontuacao po on po.ptoid = ac.ptoid
				where qfa.entid = " . $entid . " 
				and po.ptostatus = 'A' 
				and po.inuid = " . $inuid . "
				group by sa.sbaid, sa.sbadsc, fe.frmdsc, ss.ssudescricao, ss.ssuid
				order by  fe.frmdsc, ss.ssuid desc";
        
        $subacaoes = $db->carregar( $sql );
        $subacaoes = $subacaoes ? $subacaoes : array();
        $codigo_subacao = "sa_pai_" . $entid;

		$arSubacoesAgrupadas = array();
		foreach( $subacaoes as $arSubacao ){
			$arSubacoesAgrupadas[$arSubacao["frmdsc"]][$arSubacao["ssudescricao"]][] = $arSubacao; 
		}

        //echo 'arvore.add("relatorio", "' , $codigo , '", "Relatório PPP", "javascript:emitirRelatorioPPP(' , $entid , ');");
        ?>
        arvore.add( 'relatorio', 0, "Relatório PPP", "javascript:emitirRelatorioPPP(<?php echo $entid ?>);" );
        arvore.add( '<?= $codigo_subacao ?>', 0, "Sub-ações (<?= count( $subacaoes ) ?>)", 'javascript:void(0);' );
        
        // Formas de Execução
        <?php foreach( $arSubacoesAgrupadas as $stFormaExecucao => $arStatus ){ ?>
            
            arvore.add( '<?= $stFormaExecucao ?>', '<?php echo $codigo_subacao ?>', '<?php echo $stFormaExecucao ?>', 'javascript:void(0);' );
            
	        // Status da Subação
	        <?php foreach( $arStatus as $stStatus => $arSubacao ){ ?>
	            
	            arvore.add( '<?= $stStatus ?>', '<?php echo $stFormaExecucao ?>', '<?php echo $stStatus ." (" .count( $arSubacao ) .")" ?>', 'javascript:void(0);' );
	            
		        // Subações
		        <?php foreach( $arSubacao as $indice => $subacao ){ ?>
		            
		            arvore.add( 'sa_<?= $indice ?>', '<?php echo $stStatus ?>', '<?php echo str_replace(array("\n", "\r\n", "\r"), " ", addslashes($subacao["sbadsc"])) ?>', "javascript:alterarSubacao('<?php echo $subacao["sbaid"] ?>');" );
		            
		        <?php }?> // fim foreach Subações
	        <?php }?> // fim foreach Status da Subação
        <?php } // fim foreach Formas de Execução
    }

    echo '
        elemento = document.getElementById("_arvore");
        elemento.innerHTML = arvore;
    -->
    </script>';

?>


<p align="center">
  <input type="button" name="Voltar" value="Voltar" align="right" onclick="history.back();" />
</p>
