<?php
if(!$_SESSION['pse']['muncod']){
?>
<script>
    alert("É necessário informar um município\n ou sua sessão expirou. Por favor, entre novamente no PSE!");
    location.href = 'pse.php?modulo=principal/listaComp3&acao=A';
</script>
<?php
exit;
}


//pega entid da SECRETARIA MUNICIPAL DE EDUCAÇÃO de um determinado municipio
$sql = "SELECT mundescricao as municipio, estuf as uf
		FROM territorios.municipio
		WHERE muncod = '".$_SESSION['pse']['muncod']."'";
$mun = $db->pegaLinha($sql);

$sql = "select et.entid, et.entnome from entidade.entidade et
		inner join entidade.endereco ed on ed.entid=et.entid
		inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
		where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'";
$secretaria = $db->pegaLinha($sql);

$entid = $secretaria['entid'];
$conano = $_SESSION["exercicio"];


//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
//if($vpflcod == SUPER_USUARIO || $vpflcod == SECRETARIA_ESTADUAL || $vpflcod == SECRETARIA_MUNICIPAL){

if($vpflcod == SUPER_USUARIO || $vpflcod == SECRETARIA_MUNICIPAL ||  $vpflcod == SECRETARIA_ESTADUAL){
$vPermissao = 'S';
}
else{
$vPermissao = 'N';
}


//apos 31 de novembro, permite so vizualizar os dados
//if( date('Ymd') > 20121130 ) $vPermissao = 'N';


if($_POST){

//$sql = "DELETE FROM pse.monitoracomp3 WHERE muncod = '".$_SESSION['pse']['muncod']."'";
//$db->executar($sql);

$sql = "select acaoid
			from pse.acao 
			where comid = 3
			and acaoano = 2013 
			order by 1";
$dados = $db->carregar($sql);

$i = 0;
foreach($dados as $d){

$sql = "select idmonc3 
				from pse.monitoracomp3 
				where muncod = '".$_SESSION['pse']['muncod']."'
				and acaoid = ".$d['acaoid']." and c3anopactuacao = '{$_SESSION['exercicio']}' ";
$idmonc3 = $db->pegaUm($sql);

if($idmonc3){
$sqli .= "UPDATE pse.monitoracomp3
					  SET c3qtprofsaudecapa=".($_POST['c3qtprofsaudecapa'][$i] ? $_POST['c3qtprofsaudecapa'][$i] : 'null').", 
					      c3qtprofeducapa=".($_POST['c3qtprofeducapa'][$i] ? $_POST['c3qtprofeducapa'][$i] : 'null').", 
					      c3totalcapa=".($_POST['c3totalcapa'][$i] ? $_POST['c3totalcapa'][$i] : 'null').",
                                             c3dtlancamento = NOW(), usucpf = '{$_SESSION['usucpf']}'
					  WHERE idmonc3 = {$idmonc3} ; ";
}
/*
  $sqli .= "INSERT INTO pse.monitoracomp3(iduf, muncod, acaoid, c3qtprofsaudepactuado,
  c3qtprofsaudecapa, c3qtprofedupactuado, c3qtprofeducapa,
  c3totalcapa)
  VALUES ('".$mun['uf']."',
  '".$_SESSION['pse']['muncod']."',
  ".$d['acaoid'].",
  ".($_POST['c3qtprofsaudepactuado'][$i] ? $_POST['c3qtprofsaudepactuado'][$i] : 'null').",
  ".($_POST['c3qtprofsaudecapa'][$i] ? $_POST['c3qtprofsaudecapa'][$i] : 'null').",
  ".($_POST['c3qtprofedupactuado'][$i] ? $_POST['c3qtprofedupactuado'][$i] : 'null').",
  ".($_POST['c3qtprofeducapa'][$i] ? $_POST['c3qtprofeducapa'][$i] : 'null').",
  ".($_POST['c3totalcapa'][$i] ? $_POST['c3totalcapa'][$i] : 'null')."
  ); ";
 */
$i++;
}

if($sqli){
$db->executar($sqli);
$db->commit();

unset($_POST);
print "<script>
				alert('Operação realizada com sucesso!');
				location.href='pse.php?modulo=principal/cadastroComp3&acao=A';
			   </script>";

exit();
}

}








// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';


$titulo = "Monitoramento das Ações do PSE na Escola";
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.<br>
<table width="100%" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td width="10%" style="text-align: right;" class="SubTituloDireita">UF:</td>
	<td width="90%" class="SubTituloEsquerda">'.$mun['uf'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">MUNICÍPIO:</td>
	<td class="SubTituloEsquerda">'.$mun['municipio'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">SECRETARIA:</td>
	<td class="SubTituloEsquerda">'.$secretaria['entnome'].'</td>
</tr>
</table>';

echo "<br>";
/*
  $menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoPactuacao&acao=A"),
  1 => array("id" => 2, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
  );

  echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
 */
monta_titulo( $titulo, $dsctitulo );
echo "<br>";

$db->cria_aba( $abacod_tela, $url, '');

/*
  if($entid){
  $sql = "SELECT conid FROM pse.contratualizacao WHERE entid = $entid";
  $conid = $db->pegaUm($sql);
  if(!$_SESSION['conid'] && $conid) $_SESSION['conid'] = $conid;
  }

  if($_SESSION['conid']){
  $sql = "SELECT  m.mpaid,
  m.mpaacao06comp1,
  m.mpaacao01comp3,
  m.mpaacao02comp3,
  m.mpaacao03comp3,
  m.mpaacao04comp3,
  m.mpaacao05comp3
  FROM
  pse.metapactuada m
  WHERE
  m.conid = {$_SESSION['conid']}";
  $dados = $db->carregar($sql);
  if($dados) extract($dados[0]);

  if(!$mpaacao01comp3 || !$mpaacao02comp3 || !$mpaacao03comp3){
  ?>
  <script>
  alert("Acesso Negado! \nNa Pactuação do Termo de Compromisso, o Componente III não foi preenchido.");
  location.href='pse.php?modulo=principal/listaMunicipiosComp3&acao=A';
  </script>
  <?
  exit;
  }


  }
 */
?>

<form id="formulario" name="formulario" action="" method="post">

    <table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
        <tr>
            <td style="background: rgb(168, 168, 168);" colspan='6'>&nbsp;</td>
        </tr>
        <tr>
            <td colspan='6' class="SubTituloEsquerda" >
                <div style="text-align:center"><span style=" font-family:'Tahoma'; color:red; font-size:15px;"><b><b>Componente III - FORMAÇÃO</b></span></div>
            </td>
        </tr>
        <tr>
            <td style="background: rgb(168, 168, 168);" colspan='6'>&nbsp;</td>
        </tr>
        <tr>
            <td width="70%" class="SubTituloCentro">AÇÃO</td>
            <td width="10%" class="SubTituloCentro">Quantidade de Profissionais da Saúde Pactuados</td>
            <td width="10%" class="SubTituloCentro">Quantidade de Profissionais da Saúde Capacitados</td>
            <td width="10%" class="SubTituloCentro">Quantidade de Profissionais da Educação Pactuados</td>
            <td width="10%" class="SubTituloCentro">Quantidade de Profissionais da Educação Capacitados</td>
            <td width="10%" class="SubTituloCentro">Total de Profissionais Capacitados</td>
        </tr>
<?php
$sql = "select acaoid, acaodescricao
			from pse.acao 
			where comid = 3
			and acaoano = '2013' 
			order by 1";
$dados = $db->carregar($sql);

if($dados){
foreach($dados as $d){

$sql = "select idmonc3, c3qtprofsaudecapa, c3qtprofeducapa, c3qtprofsaudepactuado, c3qtprofedupactuado 
				from pse.monitoracomp3 
				where muncod = '".$_SESSION['pse']['muncod']."'
                                and c3anopactuacao = '{$_SESSION['exercicio']}'
				and acaoid = ".$d['acaoid'];
                                //var_dump($sql );
$comp3 = $db->pegaLinha($sql);
?>
        <tr>
            <td class="SubTituloEsquerda"><?= $d['acaodescricao'] ?></td>
            <td class="SubTituloCentro" nowrap="nowrap" >
                <?
                echo campo_texto( 'c3qtprofsaudepactuado[]', 'N', 'N', '', 18, 6, '######', '', 'right', '', 0, '', '', $comp3['c3qtprofsaudepactuado'], '');
                ?>
            </td>
            <td class="SubTituloCentro" nowrap="nowrap" >
                <?
                echo campo_texto( 'c3qtprofsaudecapa[]', 'S', $vPermissao, '', 18, 6, '######', '', 'right', '', 0, 'id="c3qtprofsaudecapa_'.$d['acaoid'].'"', 'somaQtd('.$d['acaoid'].')', $comp3['c3qtprofsaudecapa'], '');
                ?>
            </td>
            <td class="SubTituloCentro" nowrap="nowrap" >
                <?
                echo campo_texto( 'c3qtprofedupactuado[]', 'N', 'N', '', 18, 6, '######', '', 'right', '', 0, '', '', $comp3['c3qtprofedupactuado'], '');
                ?>
            </td>
            <td class="SubTituloCentro" nowrap="nowrap" >
                <?
                echo campo_texto( 'c3qtprofeducapa[]', 'S', $vPermissao, '', 18, 6, '######', '', 'right', '', 0, 'id="c3qtprofeducapa_'.$d['acaoid'].'"', 'somaQtd('.$d['acaoid'].')', $comp3['c3qtprofeducapa'], '');
                ?>
            </td>
            <td class="SubTituloCentro" nowrap="nowrap" >
                <?
                echo campo_texto( 'c3totalcapa[]', 'N', 'N', '', 18, 6, '######', '', 'right', '', 0, 'id="c3totalcapa_'.$d['acaoid'].'"', '', ($comp3['c3qtprofsaudecapa']+$comp3['c3qtprofeducapa']), '');
                ?>
            </td>
        </tr>
        <?php
        }
}
        ?>


        <tr>
            <td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
                <?if($vPermissao == 'S'){?>
                <input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaForm();">
                <?}?>
            </td>
        </tr>
    </table>


</form>

<script type="text/javascript">

    var d = document.formulario;

    function validaForm() {
        d.submit();
    }


    function somaQtd(id) {
        var v1 = document.getElementById("c3qtprofsaudecapa_" + id).value;
        var v2 = document.getElementById("c3qtprofeducapa_" + id).value;

        if (!v1)
            v1 = 0;
        if (!v2)
            v2 = 0;

        document.getElementById("c3totalcapa_" + id).value = (parseFloat(v1) + parseFloat(v2));
    }


</script>