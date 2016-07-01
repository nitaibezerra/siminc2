<?
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<?
if ( $_REQUEST['acao'] == 'A' ){
	
	$entid = academico_existeentidade( $_SESSION['academico']['entid'] );
	$_SESSION['sig_var']['iscampus'] = 'nao';	
	$param = "&entidunidade=".$_SESSION["academico"]["entid"];
	
}else if ( $_REQUEST['acao'] == 'C' ){
	
	$entid = academico_existecampus( $_SESSION['academico']['entidcampus'] );
	$_SESSION['sig_var']['iscampus'] = 'sim';	
	$param = "&entidcampus=".$_SESSION["academico"]["entidcampus"];
		
}

$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
					   //PERFIL_IFESCADCURSOS, 
					   //PERFIL_IFESCADASTRO, 
					   PERFIL_IFESCADTCU,
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);

$_SESSION['sig_var']['entid'] = $entid;
$_SESSION['sig_var']['orgid'] = $_SESSION['academico']['orgid'];

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

// Monta as abas
if( $_SESSION['academico']['entidadenivel'] == 'campus' ){
	if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57277' : '57136'; // Dados Desenv
	}else{
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57268' : '57136';
	}
}else{
	if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57276' : '57135'; //Dados Desenv
	}else{
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57267' : '57135';
	}
}
#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Nota Técnica', $linha2);
#academico_monta_cabecalho_sig($entid, 'Nota Técnica');
$autoriazacaoconcursos = new autoriazacaoconcursos();

$sql = "SELECT 
			(CASE usu.usucpf WHEN '".$_SESSION['usucpf']."' 
					THEN '<center><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/notatecnica&acao={$_REQUEST['acao']}&exec_function=removernotatecnica&ntcid=' || ntc.ntcid || '\',\'Deseja realmente excluir esta nota técnica?\');\"></center>' 
					ELSE '&nbsp' 
				END) as acao0, 
			(CASE WHEN arqid IS NOT NULL THEN 
				'<center><img src=\"../imagens/pdf.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarnotatecnicapdf&acao={$_REQUEST['acao']}&ntcid='|| ntc.ntcid ||'\',\'Nota\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>'
			ELSE '&nbsp' END) as acao2, 
			'<center><img src=\"../imagens/ico_html.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarnotatecnica&acao={$_REQUEST['acao']}&ntcid='|| ntc.ntcid ||'\',\'Nota\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acao, 
			to_char(ntc.ntcid, '9999999000') AS ntcid, 
			to_char(ntcdatageracao,'dd/mm/yyyy HH24:MI:SS'), 
			usunome 
		FROM 
			academico.notatecnica ntc
		LEFT JOIN 
			seguranca.usuario usu ON usu.usucpf = ntc.usucpf 
		WHERE 
			ntc.entid = '". $entid ."' 
		ORDER BY 
			ntcdatageracao DESC";
//dbg($sql, 1);
$cabecalho = array("Ações","Nota técnica (PDF)","Nota técnica (HTML)","Número","Emitido em","Emitido por");


?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <?
    echo $autoriazacaoconcursos->cabecalho_entidade($entid);
    $db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '95%','N');
    ?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    <tr bgcolor="#C0C0C0">
        <td colspan="2">
            <div style="float: left;">
            &nbsp;
            <? if($permissoes['gravar']) { ?>
                <input type="button" name="nt" id="notatecnica" value="Gerar nota técnica" onclick="window.open('?modulo=principal/visualizarnotatecnica&acao=A','Nota','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=yes,location=no');"/>
            <? } ?>
            </div>
        </td>
    </tr>
    </table>
</div>
