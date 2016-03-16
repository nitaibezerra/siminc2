<?PHP
    include "config.inc";
    header('Content-Type: text/html; charset=iso-8859-1');
    include APPRAIZ."includes/classes_simec.inc";
    include APPRAIZ."includes/funcoes.inc";
    $db = new cls_banco();

    $usucpf = $_REQUEST["usucpf"];
    $pflcod = $_REQUEST["pflcod"];

    if( !$pflcod && !$usucpf ){
?>
        <font color="red">Requisição inválida</font><?
	exit();
    }
    
    $sqlResponsabilidadesPerfil = "
        SELECT tr.* FROM maismedicomec.tprperfil p

        INNER JOIN maismedicomec.tiporesponsabilidade tr ON p.tprcod = tr.tprcod

	WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'

        ORDER BY tr.tprdsc
    ";
    $query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
    $responsabilidadesPerfil = $db->carregar($query);


    if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
        print "<font color='red'>Não foram encontrados registros</font>";
    }else{
        
	foreach ($responsabilidadesPerfil as $rp) {
            // monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
            $sqlRespUsuario = "";
            
            switch ($rp["tprsigla"]) {	
                case "M":
                    $aca_prg = "'Município'";
                    $sqlRespUsuario = "
                        SELECT  m.muncod as codigo,
                                m.mundescricao as descricao
                        FROM territorios.municipio m 
                        INNER JOIN maismedicomec.usuarioresponsabilidade ur ON ur.muncod = m.muncod
                        WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
                    ";
                    break;
                case "N":
                    $aca_prg = "'Mantenedora'";

//                    SELECT  m.mntid as codigo,
//                                                    m.mntrazaosocial as descricao
//                                            FROM maismedicomec.mantenedora m
//                                            INNER JOIN maismedicomec.usuarioresponsabilidade ur ON ur.mntid = m.mntid
//                                            WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'

                    $sqlRespUsuario = "

                        SELECT  DISTINCT m.mntid as codigo,
                        	m.mntdsc as descricao
                        FROM maismedicomec.usuarioresponsabilidade ur
                        INNER JOIN gestaodocumentos.mantenedoras m ON m.mntid = ur.mntid
                        WHERE ur.rpustatus='A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
                    ";
                    break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
                    print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem $aca_prg para este Perfil.</font>";
		}else {
?>
                    <table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
                        <tr>
                            <td colspan="3"><?= $rp["tprdsc"] ?></td>
                        </tr>
                        <tr style="color:#000000;">
                            <td valign="top" width="12">&nbsp;</td>
                            <td valign="top">Código</td>
                            <td valign="top">Descrição</td>
                        </tr>
                        <? foreach ($respUsuario as $ru) { ?>
                            <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
                                <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
                                <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?= $ru["codigo"]; ?>
                                </td>
                                <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?= $ru["descricao"] ?></td>
                            </tr>
                        <? } ?>
                        <tr>
                            <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
                                Total: (<?= @count($respUsuario) ?>)
                            </td>
                        </tr>
                    </table>
<?PHP
            }
        }
    }
    
    $db->close();
    exit();
    
?>