<?PHP
    header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
    header("Cache-Control: post-check=0, pre-check=0", false);
    header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
    header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

     /*
       Sistema Simec
       Setor responsável: SPO-MEC
       Desenvolvedor: Equipe Consultores Simec
       Analista: Cristiano Cabral
       Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
       Módulo:seleciona_unid_perfilresp.php

       */
    include "config.inc";
    header('Content-Type: text/html; charset=iso-8859-1');
    include APPRAIZ."includes/classes_simec.inc";
    include APPRAIZ."includes/funcoes.inc";
    include_once "../_constantes.php";

    $db = new cls_banco();

if ($_POST['requisicaoAjax'] == "verificaReitorExistente") {
    $entid = $_POST['entid'];
    if ($_SESSION["pflcodatribuido"] == PERFIL_REITOR) {
        $sql = "select
					usunome as nome
				from
					seguranca.usuario usu
				inner join
					academico.usuarioresponsabilidade ure ON ure.usucpf = usu.usucpf
				where
					ure.entid = $entid
				and
					ure.pflcod = " . PERFIL_REITOR . "
				and
					usu.usucpf != '" . $_SESSION['usucpf'] . "'
				and
					ure.rpustatus = 'A'
				order by
					rpudata_inc desc";
        $nome = $db->pegaUm($sql);
    }
    echo $nome ? $nome : "";
    die;
}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
if($pflcod){
	$_SESSION["pflcodatribuido"] = $pflcod;
}
$acao   = $_REQUEST["acao"];
$orgid  = $_REQUEST['orgid'];
$gravar = $_REQUEST['gravar'];
$unicod = $_REQUEST["uniresp"];

$reitor = verificaPerfil( PERFIL_REITOR );
$pro_reitor = verificaPerfil( PERFIL_PROREITOR );
$interlocutor = verificaPerfil( PERFIL_INTERLOCUTOR_INSTITUTO );

if( !$db->testa_superuser() && !$reitor && !$pro_reitor && !$interlocutor ){
    $sql = "
        SELECT oo.orgid
        FROM academico.orgao oo

        INNER JOIN academico.usuarioresponsabilidade ur ON ur.orgid = oo.orgid

        WHERE orgstatus = 'A' AND rpustatus = 'A' AND ur.entid IS NULL AND ur.usucpf = '{$_SESSION["usucpf"]}'
    ";
    $orgid = $db->pegaUm($sql);

    if (!$orgid){
        die('
            <script type="text/javascript">
                alert(\'Seu perfil não permite liberar acesso ao sistema!\');
                window.close();
            </script>
        ');
    }
}

if ($orgid == 1){
    $funid = '12';
}elseif ($orgid == 2){
    $funid = '11,14';
}elseif ($orgid == 3){
    $funid = '102';
}

if ($_POST && $gravar == 1){
	atribuiUnidade($usucpf, $pflcod, $unicod);
}

function recuperaOrgao ($orgid = null){
	global $db;

	if ( $db->testa_superuser() || verificaPerfil( PERFIL_REITOR ) ){

		$sql = "SELECT
					orgdesc
				FROM
					academico.orgao
				WHERE
					orgstatus = 'A' AND
					orgid = {$orgid}";

	}else{

		$sql = "SELECT
					orgdesc
				FROM
					academico.orgao oo
				INNER JOIN
					academico.usuarioresponsabilidade ur ON
					oo.orgid = ur.orgid
				WHERE
					orgstatus = 'A' AND
					rpustatus = 'A' AND
					usucpf = '{$_SESSION["usucpf"]}'";

	}


	return $dsc = $db->pegaUm($sql);
}

    /**
     * Função que lista as unidades
     *
     */
    function listaUnidades(){
	global $db, $funid, $orgid;

        $reitor = verificaPerfil( PERFIL_REITOR );
        $pro_reitor = verificaPerfil( PERFIL_PROREITOR );
        $interlocutor = verificaPerfil( PERFIL_INTERLOCUTOR_INSTITUTO );

	$where  = array();
	$campo  = array();
	$from   = array();

        if ( ( $db->testa_superuser() || $reitor || $pro_reitor || $interlocutor ) && !$orgid ){
            echo "
                <tr>
                    <td style='color:red;'>Faça sua busca...</td>
		</tr>
            ";
            return;
	}

	#SQL para buscar unidades existentes
	if ($db->testa_superuser() || $reitor || $pro_reitor || $interlocutor ) {
            //CARREGA as entidades do perfil reitor
            $sql = "SELECT DISTINCT entid FROM academico.usuarioresponsabilidade WHERE rpustatus = 'A' and pflcod IN ( ".PERFIL_REITOR.", ".PERFIL_PROREITOR.", ".PERFIL_INTERLOCUTOR_INSTITUTO." ) AND usucpf = '".$_SESSION['usucpf']."'";
            $arrEntidReitor2 = $db->carregarColuna($sql);

            if($arrEntidReitor2[0]){
                $arrEntidReitor = implode(",",$arrEntidReitor2);
                $andEntid = " and e.entid in (".$arrEntidReitor.") ";
            }
            //FIM CARREGA

            $unidadesExistentes = $db->carregar("
                SELECT  DISTINCT e.entid,
                        " . implode("", $campo) . "
                        e.entnome

                FROM  entidade.entidade e " . implode("",$from) . "

                INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid

                WHERE entstatus='A' AND fen.funid IN ('".str_replace(",","','",$funid)."') ".($where ? " AND ".implode(" AND ", $where) : '')." $andEntid

                ORDER BY  entnome
            ");
	} else {
		$unidadesExistentes = $db->carregar("
                    SELECT  DISTINCT e.entid,
                            e.entnome

                    FROM entidade.entidade e

                    INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
                    INNER JOIN academico.orgaofuncao of ON of.funid = fe.funid
                    INNER JOIN academico.usuarioresponsabilidade ur ON ur.orgid = of.orgid AND ur.rpustatus = 'A'

                    WHERE usucpf = '{$_SESSION["usucpf"]}'

                    ORDER BY 2;
            ");
	}

	if (!$unidadesExistentes){
            echo "
                <tr>
                    <td style='color:red;'>Busca não retornou registros...</td>
                </tr>
            ";
            return;
	}
	$count = count($unidadesExistentes);

	$orgdsc = recuperaOrgao($orgid);
	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
            $codigo    = $unidadesExistentes[$i]["entid"];
            $descricao = $unidadesExistentes[$i]["entnome"];
            $municipio = $unidadesExistentes[$i]["mundescricao"];
            $codigoUf  = $unidadesExistentes[$i]["estuf"];

            if (fmod($i,2) == 0){
                $cor = '#f4f4f4';
            } else {
                $cor='#e0e0e0';
            }

            echo "
                <tr bgcolor=\"".$cor."\">
                    <td align=\"right\" width=\"10%\">
                        <input type=\"Checkbox\" name=\"unicod\" id=\"".$codigo."\" value=\"$orgid|$codigo\" onclick=\"retorna('".$i."');\">
                        <input type=\"hidden\" name=\"unidsc\" value=\"".($funid ==1 ? $orgdsc . " - " . $descricao . " - " . $municipio . " - " .  $codigoUf : $descricao . " - " . $orgdsc)."\">
                    </td>
                    <td>
                        ".$descricao."
                    </td>
                </tr>
            ";
	}
    }

/**
 * Função que atribui a responsabilidade de uma unidade ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $unicod
 */
function atribuiUnidade($usucpf, $pflcod, $entid){

	global $db;

	$data = date("Y-m-d H:i:s");


	if(is_array($entid)){
		$arrEntid = $entid;
	}else{
		$arrEntid[] = $entid;
	}
	if($arrEntid){
		foreach($arrEntid as $ent){
			$arrEntidade = explode("|",$ent);
			$arrEntidFinal[] = $arrEntidade[1];
		}
	}
	if($pflcod==PERFIL_REITOR){
		//Remove os outros usuários da atribuição de Reitor daquela Unidade
        if (is_array($entid) && !empty($entid[0])){
            $sql = "UPDATE
                academico.usuarioresponsabilidade
                SET rpustatus = 'I'
                WHERE entid in ('".implode("','",$arrEntidFinal)."')
                AND pflcod = ".PERFIL_REITOR."";
            $db->executar($sql);
        }
	}

	$db->executar("UPDATE
					academico.usuarioresponsabilidade
				   SET
					rpustatus = 'I'
				   WHERE
					usucpf = '{$usucpf}'
					AND pflcod = '{$pflcod}'
					AND entid not in (SELECT entid FROM entidade.funcaoentidade WHERE funid in (17,18))
					-- prsano = '{$_SESSION["exercicio"]}' AND
					AND entid IS NOT NULL");

	if (is_array($entid) && !empty($entid[0])){
		$count = count($entid);

		// Insere a nova unidade
		$sql_insert = "INSERT INTO academico.usuarioresponsabilidade (
							entid,
							usucpf,
							rpustatus,
							rpudata_inc,
							pflcod -- ,
							-- prsano
					   )VALUES";

		for ($i = 0; $i < $count; $i++){
			list(,$entidade) = explode("|", $entid[$i]);

			$arrSql[] = "(
							'{$entidade}',
							'{$usucpf}',
							'A',
							'{$data}',
							'{$pflcod}' -- ,
						--	'{$_SESSION["exercicio"]}'
						 )";


		}
		$sql_insert = (string) $sql_insert.implode(",",$arrSql);

		$db->executar($sql_insert);
	}
	$db->commit();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>");

}

    function buscaUnidadesCadastradas($usucpf, $pflcod){
	global $db, $unicod;

	if( !$_POST['gravar'] && $_REQUEST["uniresp"][0] ){
            foreach ($_REQUEST["uniresp"] as $v){
                list(,$entid[]) = explode('|', $v );
            }
            $where = " e.entid IN (".implode(',',$entid).")";

	}else{
            $where = " (ur.usucpf = '{$usucpf}' AND  ur.pflcod = {$pflcod} ) ";
	}

	$sql = "SELECT DISTINCT
				e.entid as codigo,
				e.entnome as descricao,
				min(fen.funid) as funid
			FROM
		    	entidade.entidade e
		    INNER JOIN
		    	entidade.funcaoentidade fen ON fen.entid = e.entid and fen.funid not in (17,18)
		    INNER JOIN
		    	academico.usuarioresponsabilidade ur ON e.entid = ur.entid AND
														ur.rpustatus = 'A'
			WHERE
			 ".$where."
			 group by e.entid, e.entnome
			 ";

	$RS = @$db->carregar($sql);

	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {

			for ($i=0; $i<=$nlinhas;$i++) {

				foreach($RS[$i] as $k=>$v){
					${$k}=$v;
				}

				//if($codigo_old!=$codigo) {
					if ($funid == 12) {
						$orgid = 1;
					} elseif ($funid == 11 || $funid == 14) {
						$orgid = 2;
					} else {
						$orgid = 3;
					}

					$orgdsc[$funid] = $orgdsc[$funid] ? $orgdsc[$funid] : recuperaOrgao($orgid);

					print " <option value=\"$orgid|$codigo\">{$orgdsc[$funid]} - $descricao</option>";
				//}

				//$codigo_old = $codigo;

			}
		}
	} else{
		print '<option value="">Clique faça o filtro para selecionar.</option>';

	}

}

?>
<html>
    <head>
        <meta http-equiv="Pragma" content="no-cache">
        <title>Unidades</title>
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
        <link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
        <link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
    </head>
    <body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
        <div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
            <font color=blue size="2">Aguarde! Carregando Dados...</font>
        </div>
        <? flush(); ?>
        <form name="formulario" action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
            <table style="width:100%; display:none;" id="filtro" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="subtitulodireita">Tipo de Ensino:</td>
                    <td>
                        <?PHP
                            if ($db->testa_superuser() || $reitor || $pro_reitor || $interlocutor) {
                                $sql = "
                                    SELECT  orgid AS codigo,
                                            orgdesc AS descricao

                                    FROM academico.orgao
                                    WHERE orgstatus = 'A'
                                ";
                                $db->monta_combo('orgid', $sql, 'S', "-- Selecione para filtrar --", 'filtroFunid', '');
                            } else {
                                $sql = "
                                    SELECT  oo.orgid AS codigo,
                                            orgdesc AS descricao
                                    FROM academico.orgao oo

                                    INNER JOIN academico.usuarioresponsabilidade ur ON ur.orgid = oo.orgid

                                    WHERE orgstatus = 'A' AND rpustatus = 'A' AND usucpf = '{$_SESSION["usucpf"]}'
                                ";
                                $db->monta_combo('orgid', $sql, 'S', "", 'filtroFunid', '');
                            }
                            echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';
                        ?>
                    </td>
                </tr>

                <? if ($orgid == 3): ?>

                    <tr>
                        <td class="subtitulodireita">Unidade Federativa:</td>
                        <td>
                            <?PHP
                            $sql = "SELECT
						 estuf AS codigo,
						 estuf || ' - ' || estdescricao AS descricao
						FROM
						 territorios.estado
						ORDER BY
						 estuf";

                            $db->monta_combo('estuf', $sql, 'S', "-- Selecione para filtrar --", 'filtroFunid', '');
                            echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';
                            ?>
                        </td>
                    </tr>
                    <? if ($estuf): ?>
                        <tr>
                            <td class="subtitulodireita">Município:</td>
                            <td>
                                <?PHP
                                $sql = "SELECT
						 muncod AS codigo,
						 mundescricao AS descricao
						FROM
						 territorios.municipio
						WHERE
						 estuf = '{$estuf}'
						ORDER BY
						 mundescricao";

                                $db->monta_combo('muncod', $sql, 'S', "-- Selecione para filtrar --", 'filtroFunid', '');
                                ?>
                            </td>
                        </tr>
                    <? endif; ?>
                <? endif; ?>
            </table>
            <!-- Lista de Unidades -->
            <div id="tabela" style="overflow:auto; width:496px; height:270px; border:2px solid #ececec; background-color: #ffffff;">
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                    <script language="JavaScript">
                            //document.getElementById('tabela').style.visibility = "hidden";
                            document.getElementById('tabela').style.display  = "none";
                    </script>
                    <thead>
                        <tr>
                            <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Unidade(s)</strong></td>
                        </tr>
                    </thead>
                    <?PHP listaUnidades(); ?>
                </table>
            </div>
            <script language="JavaScript">
                    document.getElementById('filtro').style.display = 'block';
            </script>
            <!-- Unidades Selecionadas -->
            <input type="hidden" name="usucpf" value="<?= $usucpf ?>">
            <input type="hidden" name="pflcod" value="<?= $pflcod ?>">
            <select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">
                <?PHP
                buscaUnidadesCadastradas($usucpf, $pflcod);
                ?>
            </select>
            <!-- Submit do Formulário -->
            <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                <tr bgcolor="#c0c0c0">
                    <td align="right" style="padding:3px;" colspan="3">
                        <input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect); document.getElementsByName('gravar')[0].value=1; document.formulario.submit();" id="ok">
                        <input type="hidden" name="gravar" value="">
                    </td>
                </tr>
            </table>
        </form>

        <script type="text/javascript">

            document.getElementById('aguarde').style.visibility = "hidden";
            document.getElementById('aguarde').style.display  = "none";
            //document.getElementById('tabela').style.visibility = "visible";
            document.getElementById('tabela').style.display  = 'block';


            var campoSelect = document.getElementById("uniresp");

            <?
            if ($funid):
                ?>
                if (campoSelect.options[0].value != ''){
                for(var i=0; i<campoSelect.options.length; i++){
                var id = campoSelect.options[i].value.split('|');

                if (document.getElementById(id[1])){
                document.getElementById(id[1]).checked = true;
                }
                }
                }
                <?
            endif;
            ?>


            function abreconteudo(objeto)
            {
            if (document.getElementById('img'+objeto).name=='+')
            {
            document.getElementById('img'+objeto).name='-';
            document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
            document.getElementById(objeto).style.visibility = "visible";
            document.getElementById(objeto).style.display  = "";
            }
            else
            {
            document.getElementById('img'+objeto).name='+';
            document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
            document.getElementById(objeto).style.visibility = "hidden";
            document.getElementById(objeto).style.display  = "none";
            }
            }



            function retorna(objeto)
            {
            if(document.getElementsByName("unicod").length == 1) {
            var check_id = document.formulario.unicod;
            }else{
            var check_id = document.formulario.unicod[objeto];
            }


            if(check_id.checked == true){
            verificaReitorExistente(check_id.id,objeto);
            }else{
            insereReitor(objeto);
            }
            }

            function insereReitor(objeto)
            {
            if(document.getElementsByName("unicod").length == 1) {
            tamanho = campoSelect.options.length;
            if (campoSelect.options[0].value=='') {tamanho--;}
            if (document.formulario.unicod.checked == true){
            campoSelect.options[tamanho] = new Option(document.formulario.unidsc.value, document.formulario.unicod.value, false, false);
            sortSelect(campoSelect);
            }
            else {
            for(var i=0; i<=campoSelect.length-1; i++){
            if (document.formulario.unicod.value == campoSelect.options[i].value)
            {campoSelect.options[i] = null;}
            }
            if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);}
            sortSelect(campoSelect);
            }
            }else{
            tamanho = campoSelect.options.length;
            if (campoSelect.options[0].value=='') {tamanho--;}
            if (document.formulario.unicod[objeto].checked == true){
            campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.unicod[objeto].value, false, false);
            sortSelect(campoSelect);
            }
            else {
            for(var i=0; i<=campoSelect.length-1; i++){
            if (document.formulario.unicod[objeto].value == campoSelect.options[i].value)
            {campoSelect.options[i] = null;}
            }
            if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);}
            sortSelect(campoSelect);
            }
            }

            }

            function verificaReitorExistente(entid,objeto)
            {
            $.ajax({
            type: "POST",
            url: "cadastro_responsabilidade_unidade.php?pflcod=<?= $pflcod ?>&usucpf=<?= $usucpf ?>",
            data: "requisicaoAjax=verificaReitorExistente&entid="+entid,
            success: function(msg){
            if(msg){
            if(confirm("O(A) Reitor(a) atual desta unidade é "+msg+". Deseja substituí-lo(a)?"))
            {
            insereReitor(objeto);
            }else{
            $("#"+entid).attr("checked",false);
            }
            }else{
            insereReitor(objeto);
            }
            }
            });
            }

            function moveto(obj) {
            if (obj.options[0].value != '') {
            if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
            abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
            }
            document.getElementById(obj.value).focus();}
            }

            function filtroFunid (id) {
                var d       = document;
                var orgid   = d.getElementsByName('orgid')[0]  ? d.getElementsByName('orgid')[0].value : '';
                var estuf   = d.getElementsByName('estuf')[0]  ? d.getElementsByName('estuf')[0].value : '';;
                var muncod  = d.getElementsByName('muncod')[0] ? d.getElementsByName('muncod')[0].value : '';

                if ( !orgid ){
                    alert('Selecione um "tipo de ensino" afim de efetuar o filtro!');
                    return false;
                }

                selectAllOptions(campoSelect);
                d.formulario.submit();
                //window.location.href = '?pflcod=<?= $_GET['pflcod']; ?>&usucpf=<?= $_GET['usucpf']; ?>&funid='+funid+'&estuf='+estuf+'&muncod='+muncod;
            }

            function limpaMuncod(){
            if (document.getElementsByName('muncod')[0]) {
            document.getElementsByName('muncod')[0].value='';
            }
            }
        </script>
    </body>
</html>
