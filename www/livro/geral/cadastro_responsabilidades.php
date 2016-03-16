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
        SELECT tr.* FROM livro.tprperfil p
        
        INNER JOIN livro.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
        
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
                case "E":
                    $aca_prg = "'Editoras'";
                    $sqlRespUsuario = "
                        SELECT  e.edtid as codigo,
                                e.edtnome as descricao
                        FROM livro.editora e 
                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.edtid = ur.edtid)
                        
                        WHERE e.edtstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
                    ";
                    break;
		case "J":
                    $aca_prg = "'Editoras EJA'";
                    $sqlRespUsuario = "
                        SELECT  e.ediid as codigo,
				e.edinome as descricao
                        FROM livro.ejaeditora e 
                        
                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.ediid = ur.ediid)
                        
			WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
                    ";
                    break;
                    
		case "N":
			
			$aca_prg = "'Editoras PNIAC'";
			$sqlRespUsuario = "
                        SELECT  
                        	e.edpid as codigo,
							e.edpnomefantasia as descricao
                        FROM livro.pnaiceditora e
                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.edpid = ur.edpid)
			
			WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'";
			
			break;

        case "C":

            $aca_prg = "'Editoras CAMPO'";
            $sqlRespUsuario = "
                SELECT
                    e.cedid as codigo,
                    e.cedrazaosocial as descricao
                FROM livro.campoeditora e
                INNER JOIN livro.usuarioresponsabilidade ur ON (e.cedid = ur.cedid)

            WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'";

        break;

        case "L":
                    $aca_prg = "'Coleções'";
                    $sqlRespUsuario = "
                        SELECT  e.colid as codigo,
                                e.coltitulo as descricao
                        FROM livro.colecao e 
                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.colid = ur.colid)
                        
			WHERE e.colstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
                    ";
                    break;
		case "P":
                    $aca_prg = "'Componentes'";
                    $sqlRespUsuario = "
                        SELECT  e.comid as codigo,
				e.comdsc as descricao
                        FROM livro.componente e 
                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.comid = ur.comid)
                        WHERE e.comstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s'  AND ur.pflcod = '%s'
                    ";
                    break;
		case "D":
                    $aca_prg = "'Disciplina'";
                    $sqlRespUsuario = "
                        SELECT  e.comid as codigo,
				e.comdsc as descricao
                        FROM livro.componente e 
                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.comid = ur.comid)
                        WHERE e.comstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s'  AND ur.pflcod = '%s'
                    ";
                    break;
		case "U":
                    $aca_prg = "'Universidade PTA'";
                    $sqlRespUsuario = "
	                        SELECT  e.unvid as codigo,
							e.univdsc as descricao
                        FROM livro.ptauniversidade e
                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.unvid = ur.unvid)
                        WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s'  AND ur.pflcod = '%s'
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