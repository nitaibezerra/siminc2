<?php

cte_verificaSessao();

// Exlucir o parecer
if ($_REQUEST['entid']){
	$sql = "";
	$sql = $db->executar(
					"DELETE FROM cte.escolaproep 
					WHERE entid = '{$_REQUEST['entid']}'");
	
	$db->commit();
	$db->sucesso("principal/escolasproep");
	
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';
cte_montaTitulo( $titulo_modulo, '' );

$inuid = (integer) $_SESSION["inuid"];
$itrid = cte_pegarItrid( $inuid );

if ($itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
	$cte_proep = "ende.estuf = '" . cte_pegarEstuf( $_SESSION['inuid'] ) . "'"; 
}else {
	$cte_proep = "ende.muncod = '" . cte_pegarMuncod( $_SESSION['inuid'] ) . "'";
}


$sql = "
    select
        iue.entid,
        '[' || e.entcodent || '] ' || e.entnome as entnome,
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
    where
        iue.inuid = " . $inuid . "
        and entproep = true 
		AND fe.funid = 3 ";

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
                    <div id="arvore"></div>
                </div>
            </td>
        </tr>
    </tbody>
</table>

<script type="text/javascript">
<!--
	
	function incluirEscola(entid){
        if (entid)
            return windowOpen( '?modulo=principal/cadastrarescola&acao=A&busca=entnumcpfcnpj&entproep=true&entid=' + entid,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
        else
            return windowOpen( '?modulo=principal/cadastrarescola&acao=A&entproep=true','blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }
	    
    function escolherEscolas()
    {
        return windowOpen( '?modulo=principal/escolasescolha&acao=A&boProep=true','blank','height=400,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }	    
	    
    function voltar(){
        window.location.href = '?modulo=principal/estrutura_avaliacao&acao=A';
    }
    
    function excluirParecer(entid){
    	if(confirm("Deseja realmente excluir este parecer?")){
    		window.location.href = '?modulo=principal/escolasproep&acao=A&entid=' + entid;
    	}
    }
    
    function manterParecer(entid){
    	window.open('?modulo=principal/manterParecerProep&acao=A&entid=' + entid, 'blank', 'height=300,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }
    
    var arvore = new dTree( 'arvore' );
    arvore.config.folderLinks = true;
    arvore.config.useIcons = true;
    arvore.config.useCookies = true; 

    arvore.add( 1, -1, "Escolas PROEP", 'javascript:void(0);' );
    
    <?php
        $municipios_adicionados = array();
        
        
        
        foreach ( $escolas as $escola ) {
        	
            $entid        = (integer) $escola["entid"];
            $entnome      = $escola["entnome"];
			$codigo_pai	  = 1;
			
        	if ($itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL) {
			    $codigo_pai = "mu_" . $entid;
				echo 'arvore.add(\'' , $codigo_pai , '\', 1, \'' , addslashes($entnome) , '\');';
            }

            $nome   = '<a href=\\"?modulo=principal/escola&acao=A&entid=' . $entid . '\\">' . addslashes($entnome) . '</a>';
            $codigo = 'es_' . $entid;
            
            $sql = "";
            $sql = "SELECT espid FROM cte.escolaproep WHERE entid = '{$entid}'";
            
            $parecer = $db->pegaUm($sql);
            
            if ($parecer){
            	$img = ", '', '', '../imagens/check_p.gif'";
            	$exclui_parecer = '<img align="absmiddle" src="/imagens/excluir.gif" onclick="excluirParecer('.$entid.');"/> ';
            }else{
            	$img = ", '', '', '../includes/dtree/img/page.gif'";
            	$exclui_parecer = "";
            }
            $textoParecer = '<a href="javascript:manterParecer(' . $entid . ');">PARECER</a>';
		?>
		
		arvore.add('<?= $codigo ?>', '<?= $codigo_pai ?>', '<?= $exclui_parecer?><?= $textoParecer ?>', 'javascript:void(0);'<?= $img ?>);

        <?php
        continue;
        }
		?>
    
    elemento = document.getElementById( 'arvore' );
    elemento.innerHTML = arvore;
    
    
    
//-->
</script>

<p align="center">
	<input type="button" name="incluirEscola" value="Cadastrar Escola" onclick="incluirEscola();" />&nbsp;&nbsp;&nbsp;
	<input type="button" name="AddRemove" value="Adicionar / Remover" onclick="escolherEscolas();" />&nbsp;&nbsp;&nbsp;  
	<input type="button" name="Voltar" value="Voltar" align="right" onclick="voltar();" />
</p>