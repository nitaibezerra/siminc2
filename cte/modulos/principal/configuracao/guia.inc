<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

# Deletar Dimensão
if ($_GET['dimidDel']) {

    $sql_checa = 'SELECT
                    COUNT(ardid)
                  FROM 
                    cte.areadimensao
                  WHERE
                    ardstatus = \'A\' AND
                    dimid = ' . $_GET[dimidDel];

    $checa = $db->pegaUm($sql_checa);

    if ($checa) {
        echo '<script type="text/javascript">'
            ,'alert(\'Operação não realizada!\nExiste Área cadastrada nesta dimensão\');'
            ,'location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';'
            ,'</script>';
    } else {
        $sql_verifica = 'SELECT
                            COUNT(ardid)
                         FROM
                            cte.areadimensao
                         WHERE
                            dimid=' . $_GET['dimidDel'];

        $verifica = $db->pegaUm($sql_verifica);

        if (!$verifica) {
            $sql = 'DELETE FROM
                        cte.dimensao
                    WHERE
                        dimid = ' . $_GET['dimidDel'];
        } else {
            $sql = 'UPDATE
                        cte.dimensao
                    SET
                        dimstatus = \'I\'
                    WHERE
                        dimid = ' . $_GET['dimidDel'];
        }

        $db->executar($sql);
        $db->commit();

        echo '<script type="text/javascript">'
            ,'alert(\'Operação realizada com sucesso!\');'
            ,'location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';'
            ,'</script>';
    }

    die();
}



if ($_GET['ardidDel']):
    $sql_checa = sprintf("SELECT
                            COUNT(indid)
                          FROM 
                            cte.indicador
                          WHERE
                            indstatus = 'A' AND
                            ardid = %d",$_GET[ardidDel]);
    $checa = $db->pegaUm($sql_checa);
    if ($checa):
        die ('<script type="text/javascript">
                alert(\'Operação não realizada!\nExiste Indicador cadastrado nesta Área\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');
    else:
        $sql_verifica = sprintf("SELECT
                                    COUNT(indid)
                                 FROM
                                    cte.indicador
                                 WHERE
                                    ardid = %d",$_GET[ardidDel]);
        $verifica = $db->pegaUm($sql_verifica);

        if (!$verifica):
            $sql = sprintf("DELETE
                            FROM
                                cte.areadimensao
                            WHERE
                                ardid = %d",$_GET[ardidDel]);
        else:
            $sql = sprintf("UPDATE
                                cte.areadimensao
                            SET
                                ardstatus = 'I'
                            WHERE
                                ardid = %d",$_GET[ardidDel]);
        endif;
        $db->executar($sql);
        $db->commit();
        
        die ('<script type="text/javascript">
                alert(\'Operação realizada com sucesso!\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');
    endif;
endif;

# Deletar Indicador
if ($_GET[indidDel]):

    $sql_checa = sprintf("SELECT
                            COUNT(crtid)
                          FROM
                            cte.criterio
                          WHERE
                            indid = %d",$_GET[indidDel]);
    $checa = $db->pegaUm($sql_checa);
    if ($checa):
        die ('<script type="text/javascript">
                alert(\'Operação não realizada!\nExiste Critério cadastrado neste Indicador\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');
    else:
        # Deletar Registros cte.indicadorunidademedida
        $sql_1 = sprintf("DELETE FROM
                            cte.indicadorunidademedida
                          WHERE
                            indid = %d",$_GET[indidDel]);
        $db->executar($sql_1);
        
        # Deletar Registros cte.diretrizindicador
        $sql_2 = sprintf("DELETE FROM
                            cte.diretrizindicador
                          WHERE
                            indid = %d",$_GET[indidDel]);
        $db->executar($sql_2);
        
        # Deletar INDICADOR
        $sql = sprintf("DELETE
                        FROM
                            cte.indicador
                        WHERE
                            indid = %d",$_GET[indidDel]);
        $db->executar($sql);
        $db->commit();
        
        die ('<script type="text/javascript">
                alert(\'Operação realizada com sucesso!\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');
    endif;
endif;

# Deletar Critério
if ($_GET[crtidDel]):

    $sql_checa = sprintf("SELECT
                            COUNT(ppaid)	
                          FROM 
                            cte.proposicaoacao
                          WHERE
                            crtid = %d",$_GET[crtidDel]);
    $checa = $db->pegaUm($sql_checa);
    if ($checa):
        die ('<script type="text/javascript">
                alert(\'Operação não realizada!\nExiste Ação cadastrado neste Critério\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');	
    endif;
    
    # Verificar se há "Pontuação" vinculada ao "Critério
    $sql_checa1 = sprintf("SELECT
                             COUNT(ptoid)
                           FROM
                              cte.pontuacao
                           WHERE
                              crtid = %d", $_GET[crtidDel]);
    $checa1 = $db->pegaUm($sql_checa1);
    if ($checa1):
        die ('<script type="text/javascript">
                alert(\'Operação não realizada!\nExiste Pontuação cadastrado neste Critério\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');		
    else:
        # Deletar Critério
        $sql = sprintf("DELETE FROM
                            cte.criterio
                        WHERE
                            crtid = %d",$_GET[crtidDel]);
        $db->executar($sql);
        $db->commit();
        
        die ('<script type="text/javascript">
                alert(\'Operação realizada com sucesso!\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');
    endif;
endif;

# Deletar Ação
if ($_GET[ppaidDel]):

    $sql_checa = sprintf("SELECT
                            COUNT(ppsid)	
                          FROM 
                            cte.proposicaosubacao
                          WHERE
                            ppaid = %d",$_GET[ppaidDel]);
    $checa = $db->pegaUm($sql_checa);
    if ($checa):
        die ('<script type="text/javascript">
                alert(\'Operação não realizada!\nExiste Subação cadastrada nesta Ação\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');	
    endif;

    # Verificar se há "Ação" vinculada à "proposicaoacao"
    $sql_checa1 = sprintf("SELECT
                             COUNT(aciid)
                           FROM
                              cte.acaoindicador
                           WHERE
                              ppaid = %d", $_GET[ppaidDel]);
    $checa1 = $db->pegaUm($sql_checa1);
    if ($checa1):
        die ('<script type="text/javascript">
                alert(\'Operação não realizada!\nExiste AcãoIndicador cadastrada nesta Ação\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');		
    else:
        # Deletar proposicaoacao
        $sql = sprintf("DELETE FROM
                            cte.proposicaoacao
                        WHERE
                            ppaid = %d",$_GET[ppaidDel]);
        $db->executar($sql);
        $db->commit();
        
        die ('<script type="text/javascript">
                alert(\'Operação realizada com sucesso!\');
                location.href=\'cte.php?modulo=principal/configuracao/guia&acao=A\';
              </script>');
    endif;
endif;

if ( $_REQUEST['operacao'] ) {
    try {
        switch ($_REQUEST['operacao']) {
            case 'incluirSubacao':
                if ($_REQUEST['indid']) {
                    $sql = 'select count(*) from cte.indicador where indid = ' . (integer) $_REQUEST['indid'];
                    $ins = 'insert into cte.proposicaosubacao (indid,
                                                               ppsdsc,
                                                               ppsordem) values ('   . $_REQUEST['indid']  . ',
                                                                                 \'' . $_REQUEST['ppsdsc'] . '\',
                                                                                 ( select count(*) + 1 from cte.proposicaosubacao where indid = ' .$_REQUEST['indid']. '))';
                } 
                else {
                    $sql = 'select count(*) from cte.proposicaoacao where ppaid = ' . (integer) $_REQUEST['ppaid'];

                    $ins = sprintf("insert into cte.proposicaosubacao (ppaid,
                                                                       ppsdsc,
                                                                       ppsordem) values (%d, '%s', ( select count(*) + 1 from cte.proposicaosubacao where ppaid = %d ) )", $_REQUEST["ppaid"], $_REQUEST["ppsdsc"], $_REQUEST["ppaid"] );
                    if ($db->pegaUm( $sql ) != 1)
                        throw new Exception('A ação não existe.');
                }

                if ( !$db->executar( $ins ) ) {
                    throw new Exception('Erro ao inserir subação.');
                }
                break;

            case "excluirSubacao":
                $sql = sprintf( "select count(*) from cte.subacaoindicador where ppsid = %d", $_REQUEST["ppsid"] );
                if ( $db->pegaUm( $sql ) != 0 ) {
                    throw new Exception( "A subação está em uso." );
                }
                $sql = sprintf( "select ppaid from cte.proposicaosubacao where ppsid = %d", $_REQUEST["ppsid"] );
                $ppaid = $db->pegaUm( $sql );
                $sql = sprintf( "delete from cte.proposicaosubacao where ppsid = %d", $_REQUEST["ppsid"] );
                if ( !$db->executar( $sql ) ) {
                    throw new Exception( "Erro ao excluir subação." );
                }
                $sql = sprintf(
                    "update cte.proposicaosubacao s1 set ppsordem = (
                        select count(*) + 1
                        from cte.proposicaosubacao s2
                        where s2.ppaid = s1.ppaid and s2.ppsordem < s1.ppsordem
                    ) where ppaid = %d;",
                    $ppaid
                );
                if ( !$db->executar( $sql ) ) {
                    throw new Exception( "Erro ao reordenar o guia." );
                }
                $sql = sprintf(
                    "update cte.subacaoindicador set
                    sbaordem = ( select ppsordem from cte.proposicaosubacao where ppsid = sbaid )
                    where ppsid in ( select ppsid from cte.proposicaosubacao where ppaid = %d )",
                    $ppaid
                );
                if ( !$db->executar( $sql ) ) {
                    throw new Exception( "Erro ao reordenar as subações." );
                }
                break;
            
            default:
                break;
            
        }
        $db->commit();
        $_REQUEST["acao"] = "A";
        $db->sucesso( $_REQUEST["modulo"] );
        
    } catch ( Exception $erro ) {
        $db->rollback();
        ?>
        <html>
            <head>
                <script type="text/javascript">
                alert( '<?= $erro->getMessage(); ?>' );
                location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>";
                </script>
            </head>
            <body/>
        </html>
        <?
    }
}

$sql_instrumento = sprintf( "
    select distinct it.itrid, it.itrdsc
    from cte.instrumento it
    --inner join cte.dimensao d on d.itrid = it.itrid and d.dimstatus = 'A'
    --inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A'
    --inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A'
    --inner join cte.criterio c on c.indid = i.indid
    --inner join cte.proposicaoacao pa on pa.crtid = c.crtid
    where it.itrid not in ( 3, 4 )
    order by it.itrdsc
" );
$instrumentos = $db->carregar( $sql_instrumento );
$instrumentos = $instrumentos ? $instrumentos : array();


$sql = '
select distinct d.dimid, d.dimcod, d.dimdsc, d.itrid
from cte.dimensao d
where d.dimstatus = \'A\'
and d.itrid not in ( 3, 4 )
order by d.dimcod';

$dimensoes = $db->carregar($sql);
$dimensoes = $dimensoes ? $dimensoes : array();


$sql = '
    select distinct ad.ardid, ad.ardcod, ad.arddsc, ad.dimid
    from cte.areadimensao ad
    inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = \'A\' and d.itrid not in ( 3, 4 )
    where ad.ardstatus = \'A\'
    order by ad.ardcod';

$areas = $db->carregar($sql);
$areas = $areas ? $areas : array();


$sql = '
SELECT distinct
    ind.indid,
    ind.indcod,
    ind.inddsc,
    ind.ardid,
    dim.itrid
FROM
    cte.indicador ind
INNER JOIN
    cte.areadimensao ard on ind.ardid = ard.ardid
INNER JOIN
    cte.dimensao dim on dim.dimid = ard.dimid and dim.dimstatus = \'A\' and dim.itrid not in ( 3, 4 )
WHERE
    ind.indstatus = \'A\'
ORDER BY
    ind.indcod';

$indicadores = $db->carregar($sql);
$indicadores = $indicadores ? $indicadores : array();


$sql = '
SELECT
    distinct c.crtid,
    c.ctrord, 
    c.crtdsc, 
    c.indid, 
    c.ctrpontuacao,
    d.itrid
FROM
    cte.criterio c
INNER JOIN
    cte.indicador i on i.indid = c.indid and i.indstatus = \'A\'
INNER JOIN
    cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = \'A\'
INNER JOIN
    cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = \'A\'
WHERE
    d.dimstatus = \'A\' and d.itrid not in (3, 4)
ORDER BY
    c.ctrord';

$criterios = $db->carregar($sql);
$criterios = $criterios ? $criterios : array();



$sql = '
select
    pa.ppaid,
    pa.ppadsc,
    pa.crtid,
    d.itrid
from
    cte.proposicaoacao pa
inner join
    cte.criterio c on c.crtid = pa.crtid
inner join
    cte.indicador i on i.indid = c.indid and i.indstatus = \'A\'
inner join
    cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = \'A\'
inner join
    cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = \'A\'
where
    d.itrid not in (3,4)';

$acoes = $db->carregar($sql);
$acoes = $acoes ? $acoes : array();

$sql = '
select
    ps.ppsid,
    ps.ppaid,
    ps.ppsdsc,
    ps.ppsordem,
    d.itrid,
    i.indid,
    ps.indid
from
    cte.proposicaosubacao ps
left join
    cte.proposicaoacao pa on pa.ppaid = ps.ppaid
left join
    cte.criterio c on c.crtid = pa.crtid
inner join
    cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = \'A\'
inner join
    cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = \'A\'
inner join
    cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = \'A\'
where
    d.itrid not in (3,4)
order
    by ps.ppsordem';

$subacoes = $db->carregar($sql);
$subacoes = $subacoes ? $subacoes : array();

?>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td><a href="javascript: windowOpen('/cte/cte.php?modulo=principal/configuracao/cadastroNumerosDeProcessos&acao=A','Cadastro de Números de Processos.','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes')" >Cadastra números de processo para geração do Parecer.</a></td>
	
</tr>
</table>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
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
      <td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top"></td>
    </tr>
  </tbody>
</table>

<script type="text/javascript">
    arvore = new dTree('arvore');
    arvore.config.folderLinks = true;
    arvore.config.useIcons    = true;
    arvore.config.useCookies  = true; 

    arvore.add(0, -1, 'Guia de Ações Padronizadas');

<?php

    foreach( $instrumentos as $instrumento ) {
        echo 'arvore.add(\'it_' , $instrumento['itrid'] , '\',0,\'<img src="/imagens/gif_inclui.gif" title="Incluir Dimensão" onclick="windowOpen(\\\'cte.php?modulo=principal/configuracao/popupDimensao&acao=A&itrid=' , $instrumento['itrid'] , '\\\',\\\'Janela7\\\',\\\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=130\\\');">' , $instrumento['itrdsc'] , '\',\'javascript:void(0)\',\'\',\'\',\'../includes/dtree/img/question.gif\',\'../includes/dtree/img/question.gif\');';
    }

    foreach( $dimensoes as $dimensao ) {
        echo 'arvore.add(\'d_' , $dimensao['dimid'] , '\',\'it_' , $dimensao['itrid'] , '\',\''
            ,'<img src="/imagens/gif_inclui.gif" title="Incluir Área" onclick="windowOpen(\\\'cte.php?modulo=principal/configuracao/popupArea&acao=A&dimid=' , $dimensao['dimid'] , '\\\',\\\'Janela8\\\',\\\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=150\\\');"/>'
            ,'<img src="/imagens/alterar.gif" title="Alterar Dimensão" onclick="windowOpen(\\\'cte.php?modulo=principal/configuracao/popupDimensao&acao=A&itrid=' , $dimensao['itrid'] , '&dimid=' , $dimensao['dimid'], '\\\',\\\'Janela3434\\\',\\\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=200\\\');"/>'
            ,'<img src="/imagens/excluir.gif" title="Excluir Dimensão" onclick="confirmar(\\\'/cte/cte.php?modulo=principal/configuracao/guia&acao=A&dimidDel=' , $dimensao['dimid'] , '\\\',\\\'Deseja apagar esta Dimensão (' , $dimensao['dimdsc'] , ')\\\')"/><strong>' , $dimensao['dimcod'] , '</strong>' , $dimensao['dimdsc'] , '\',\'javascript:void(0)\',\'\',\'\',\'../includes/dtree/img/folder.gif\',\'../includes/dtree/img/folder.gif\');';
    }

    foreach( $areas as $area ) {
        $texto = "<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir Indicador\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupIndicador&acao=A&ardid={$area[ardid]}\',\'Janela9\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=690,height=480,left=10,top=100\'); w.focus(); \"> <img src=\'/imagens/alterar.gif\' title=\'Alterar Área\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupArea&acao=A&dimid={$area[dimid]}&ardid={$area[ardid]}\',\'Janela122\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=150\'); w.focus(); \"> <img src=\'/imagens/excluir.gif\' title=\'Excluir Área\' onclick=\"confirmar('cte.php?modulo=principal/configuracao/guia&acao=A&ardidDel={$area['ardid']}', 'Deseja apagar esta Área ({$area['arddsc']})?')\" /> <b>{$area['ardcod']}</b> {$area['arddsc']}";
        echo 'arvore.add(\'a_' , $area['ardid'] , '\',\'d_' , $area['dimid'] , '\',"' , str_replace('"','\"',$texto) , '", \'javascript:void(0)\',\'\',\'\',\'../includes/dtree/img/folder.gif\',\'../includes/dtree/img/folder.gif\');';
    }

    foreach($indicadores as $indicador) {
        if ($indicador['itrid'] == 1) {
//            $incluir = "<img align='absmiddle' onclick='javascript:incluirSubacao(\"\", \"" . $indicador['indid'] . "\");' style='cursor: pointer;' src='/imagens/gif_inclui.gif' title='cadastrar subação'/>";
            $incluir = "<img align='absmiddle' onclick='javascript:incluirSubacao(\"\", \"\", \"" . $indicador['indid'] . "\");' style='cursor: pointer;' src='/imagens/gif_inclui.gif' title='cadastrar subação'/>";
            $incluir .= "&nbsp<img src=\'/imagens/alterar.gif\' title=\'Alterar Indicador\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupIndicador&acao=A&ardid={$indicador[ardid]}&indid={$indicador[indid]}\',\'Janela874550\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=570,height=400,left=10,top=100\'); w.focus(); \">";
            $incluir .= "&nbsp<img src=\'/imagens/excluir.gif\' title=\'Excluir Indicador\' onclick=\"confirmar('cte.php?modulo=principal/configuracao/guia&acao=A&indidDel={$indicador[indid]}}', 'Deseja apagar este indicador ({$indicador[inddsc]})?')\" />";
            $texto = $incluir."&nbsp;<b>{$indicador["inddsc"]}</b>";
        } else {
            $texto = "<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir Critério\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupCriterio&acao=A&indid={$indicador[indid]}\',\'Janela10\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=570,height=330,left=10,top=100\'); w.focus(); \">  <img src=\'/imagens/alterar.gif\' title=\'Alterar Indicador\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupIndicador&acao=A&ardid={$indicador[ardid]}&indid={$indicador[indid]}\',\'Janela334\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=730,height=480,left=10,top=100\'); w.focus(); \"> <img src=\'/imagens/excluir.gif\' title=\'Excluir Indicador\' onclick=\"confirmar('cte.php?modulo=principal/configuracao/guia&acao=A&indidDel={$indicador[indid]}', 'Deseja apagar este indicador ({$indicador[inddsc]})?')\" /> <b>{$indicador["indcod"]}</b> {$indicador["inddsc"]}";
        }
        ?>
        arvore.add( 'i_<?= $indicador["indid"] ?>', 'a_<?= $indicador["ardid"] ?>', "<?= str_replace('"','\"',$texto) ?>",'javascript:void(0)' );
    <?php
    }

    foreach( $criterios as $criterio ) {
        if ($criterio['itrid'] == 1)
            continue;

        $texto = "<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir Ação\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupAcao&acao=A&crtid={$criterio[crtid]}\',\'Janela750\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=570,height=200,left=10,top=100\'); w.focus(); \"> <img src=\'/imagens/alterar.gif\' title=\'Alterar Critério\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupCriterio&acao=A&indid={$criterio[indid]}&crtid={$criterio[crtid]}\',\'Janela1581\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=570,height=330,left=10,top=100\'); w.focus(); \"> <img src=\'/imagens/excluir.gif\' title=\'Excluir Critério\' onclick=\"confirmar('cte.php?modulo=principal/configuracao/guia&acao=A&crtidDel={$criterio[crtid]}', 'Deseja apagar este critério ({$criterio[crtdsc]})?')\" /> {$criterio['ctrpontuacao']} {$criterio["crtdsc"]}";
        ?>
        arvore.add( 'c_<?= $criterio["crtid"] ?>', 'i_<?= $criterio["indid"] ?>', "<?= str_replace('"','\"',$texto) ?>", 'javascript:void(0)', '', '', '../includes/dtree/img/page.gif', '../includes/dtree/img/page.gif' );
<?php
    }

    foreach( $acoes as $acao ): 
        if ($acao['itrid'] == 1)
            continue;

        $incluir = "<img align='absmiddle' onclick='javascript:incluirSubacao({$acao["ppaid"]}, {$acao["crtid"]}, \"\" );' style='cursor: pointer;' src='/imagens/gif_inclui.gif' title='cadastrar subação'/>";
        $incluir .= "&nbsp<img src=\'/imagens/alterar.gif\' title=\'Alterar Ação\' onclick=\"w = window.open(\'cte.php?modulo=principal/configuracao/popupAcao&acao=A&crtid={$acao[crtid]}&ppaid={$acao[ppaid]}\',\'Janela874550\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=570,height=200,left=10,top=100\'); w.focus(); \">";
        $incluir .= "&nbsp<img src=\'/imagens/excluir.gif\' title=\'Excluir Ação\' onclick=\"confirmar('cte.php?modulo=principal/configuracao/guia&acao=A&ppaidDel={$acao[ppaid]}', 'Deseja apagar esta ação ({$acao[ppadsc]})?')\" />";
        $texto = str_replace('"','\"',$incluir)."&nbsp;<b>{$acao["ppadsc"]}</b>";
        ?>
        arvore.add( 'pa_<?= $acao["ppaid"] ?>', 'c_<?= $acao["crtid"] ?>', "<?= $texto ?>", 'javascript:void(0);', '', '', '../includes/dtree/img/vazio.gif', '../includes/dtree/img/vazio.gif' );
    <?php
        endforeach; ?>

    <?php foreach( $subacoes as $subacao ): ?>
    <?php
        if ($subacao['itrid'] == 1 || $subacao['indid'] != '') {
            // subacao 1 - indicador 1.1.1
            $editar = "<img align='absmiddle' onclick='javascript:editarSubacao({$subacao["ppsid"]});' style='cursor: pointer;' src='/imagens/alterar.gif' title='alterar subação'/>";
            $excluir = "<img align='absmiddle' onclick='javascript:excluirSubacao({$subacao["ppsid"]});' style='cursor: pointer;' src='/imagens/excluir.gif' title='excluir subação'/>";
            $texto = "{$editar}&nbsp;{$excluir}&nbsp;<b>{$subacao["ppsordem"]}</b>&nbsp;{$subacao["ppsdsc"]}";
        ?>
        arvore.add('ps_<?= $subacao["ppsid"] ?>', 'i_<?= $subacao['indid'] ?>', "<?= $texto ?>", 'javascript:void(0);', '', '', '../includes/dtree/img/vazio.gif', '../includes/dtree/img/vazio.gif');
        <?php
        } else {
            $editar = "<img align='absmiddle' onclick='javascript:editarSubacao({$subacao["ppsid"]});' style='cursor: pointer;' src='/imagens/alterar.gif' title='alterar subação'/>";
            $excluir = "<img align='absmiddle' onclick='javascript:excluirSubacao({$subacao["ppsid"]});' style='cursor: pointer;' src='/imagens/excluir.gif' title='excluir subação'/>";
            $texto = "{$editar}&nbsp;{$excluir}&nbsp;<b>{$subacao["ppsordem"]}</b>&nbsp;{$subacao["ppsdsc"]}";
?>
        arvore.add('ps_<?= $subacao["ppsid"] ?>', 'pa_<?= $subacao["ppaid"] ?>', "<?= $texto ?>", 'javascript:void(0);', '', '', '../includes/dtree/img/vazio.gif', '../includes/dtree/img/vazio.gif');

<?php
        }
    endforeach; ?>

    elemento = document.getElementById( '_arvore' );
    elemento.innerHTML = arvore;
    
</script>

<script type="text/javascript">

    function confirmar (url,mens) {
        if (confirm(mens)) {
            location.href = url;
            return true;
        }
        return false;
    }
    
    // AÇÃO
    
    function editarAcao( ppaid ){
        window.location = '?modulo=principal/configuracao/guia_acao&acao=A&ppaid=' + ppaid;
    }
        
    // SUBAÇÃO
    
    function editarSubacao( ppsid ){
        var janela = window.open(
            '?modulo=principal/configuracao/guia_subacao&acao=A&ppsid=' + ppsid,
            'proposicaosubacao',
            'height=600,width=700,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
        );
        janela.focus();
    }
    
    function incluirSubacao( ppaid, crtid, indid ){
        var janela = window.open(
            '?modulo=principal/configuracao/guia_subacao&acao=A&crtid=' + crtid + '&ppaid=' + ppaid + '&indid=' + indid,
            'proposicaosubacao',
            'height=600,width=700,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
        );
        janela.focus();
    }
    
    function excluirSubacao( ppsid ){
        if ( confirm( "Deseja excluir a subação?" ) ) {
            window.location = '?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>&operacao=excluirSubacao&ppsid=' + ppsid;
        }
    }

</script>