<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:monitoraprg.inc
   finalidade: permitir o monitoramento de programa
   */

 //if (!$db->testa_superuser()) {print '<script>alert("Página em Construção!");history.back();</script>';exit();}
 //Aumenta a sessão da página
ini_set('session.gc_maxlifetime', 4800);


if(is_array($_POST['acaidvalida']) && @count($_POST['acaidvalida'])>0 && $_POST['corcod_par'])
{
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$sqlInsAvp = "Insert into avaliacaoparecer (refcod,usucpf,corcod,acaid,tpaid,avptexto,avpliberada,avpstatus,avporigem,avpdata) VALUES('%s', '%s', '%s', '%s', 2, '%s', '1', 'A', 4, now())";
	$sqlUpdAvp = "Update avaliacaoparecer Set refcod='%s', usucpf='%s', corcod='%s', acaid='%s', tpaid=2, avptexto='%s', avpliberada='1', avpstatus='A', avporigem=4, avpdata=now() Where avpid='%s'";
	
	foreach ($_POST['acaidvalida'] as $acaidvalida) {
		if($acaidvalida){
			$avpid = $db->pegaUm("select avpid from avaliacaoparecer where tpaid=2 and refcod=".$_POST['refcod']." and acaid=".$acaidvalida);
	
			if (!$avpid) {
				$dadosConsulta = array($_POST["refcod"], $_SESSION["usucpf"], $_POST["corcod_par"], $acaidvalida, $_POST["avptexto"][$acaidvalida]);
				$sql = vsprintf($sqlInsAvp, $dadosConsulta);
			}
			else {
				$dadosConsulta = array($_POST["refcod"], $_SESSION["usucpf"], $_POST["corcod_par"], $acaidvalida, $_POST["avptexto"][$acaidvalida], $avpid);
				$sql = vsprintf($sqlUpdAvp, $dadosConsulta);			
			}
			$db->executar($sql);
		}
	}
	$db->commit();
	//$db->sucesso($modulo);
	print '<script>alert("Operação Realizada com Sucesso!");history.back();</script>';
}

$modulo=$_REQUEST['modulo'] ;
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];

include "includes/cabecalho.inc";
print "<br>";
//Rola a tela para caber em 800x600
$ajustaTela = true;
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Avaliação de Programa';
monta_titulo($titulo_modulo,'Validação do Gerente <span id="referencia"> -- </span>');

if ($_POST['prgcod']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['prgcod']."'  order by prgcod";
elseif ($_REQUEST['prgid']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
// as linhas abaixo não permitem navegar para além dos limites
//controla_navegacao($nlinhas);
if ($nlinhas < 0 ){
	?>
	<script>alert('Programa Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
$res =  $db->carrega_registro($RS,0);
// a linha abaixo transforma em variÃ¡veis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
	$_SESSION['prgid']=$prgid;
	    // encontrar o orgao
    $sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a unidade
    $sql= "select unidsc from unidade where unicod='".$unicod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if ($_REQUEST['refcod']) $refcod=$_REQUEST['refcod'];
if ($_REQUEST['unicod']) $unicod=$_REQUEST['unicod'];
if ($_REQUEST['corcod'] or $_REQUEST['corcod']==0) $corcod=$_REQUEST['corcod'];


//faz o controle do registro atual para a paginação
if (!$_REQUEST['numero']) $numero = 1; else $numero = intval($_REQUEST['numero']);

//Teste se é Gerente (2) ou Gerente Executivo (3)
$gerente = false;
if ($db->testa_responsabilidade($prgid, 2, 'prgid')) $gerente=true;
if ($db->testa_responsabilidade($prgid, 3, 'prgid')) $gerente=true;
if ($db->testa_superuser()) $gerente=true;

if ($refcod){
$refdata_limite_parecer_aca = $db->pegaUm("select refdata_limite_parecer_aca from referencia where refcod=".$refcod);

	if ($gerente and $refdata_limite_parecer_aca < date('Y-m-d'))
   {
      $gerente = false;
     // não está autorizado na data
     // então testa se tem alguma autorização especial
     if ($refcod){
       $sql= "select ae.aelid from autorizacaoespecial ae where ae.prgid =".$_SESSION['prgid']." and ae.aelstatus ='A' and ae.aeldata_inicio <='".date('Y-m-d')."' and ae.aeldata_fim >='".date('Y-m-d')."' and ae.togcod in (1,2) and ae.refcod=".$refcod;

     $registro=$db->recuperar($sql);
     if (is_array($registro)) $gerente = true;
     }

   }
}
   // a variavel HABIL indica se esta dentro da data



if ($gerente) {
	$largura='width:99.9%';

?>
<center>
<DIV id="valida" style="OVERFLOW:AUTO; WIDTH:95%; HEIGHT:220px;border-bottom: 2px solid #cccccc;">
<?}?>
<form method="POST"  name="formulario" style="margin:0px;">
<input type=hidden name="corcod_par" value="">
<input type=hidden name="numero" value="1">


<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;<?=$largura?>">
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Código:</td>
        <td><?=campo_texto('prgcod','N','S','',10,100,'','');?>
	&nbsp;&nbsp;<img border="0" src="imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="procura()">
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Programa:</td>
        <td><?if ($refcod) print $db->grafico_validacao_prg($prgid,$refcod);?> <?=$prgdsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Orgão Responsável:</td>
        <td><?=$orgdsc?></td>
      </tr>
      <?if ($unidsc) {?>
      <tr>
        <td align='right' class="subtitulodireita">Unidade Responsável:</td>
         <td><?=$unidsc?></td>
      </tr>
      <?}?>
<?
   $sql = "select pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if (! $mostra ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }
        }
     }
     if ($refcod) $funcao_combo="document.formulario.submit";
?>
	<tr><td align='right' class="SubTituloDireita">Período de Referência :</td>
		<td>
		<?
		$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_parecer_aca is not null and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
			$db->monta_combo("refcod",$sql2,'S',"Escolha o período de referência",$funcao_combo,'');
		
		?>
		</td>
	</tr>
	<tr>
        <td align='right' class="subtitulodireita">Unidade Orçamentária:</td>
         <td>
         	<?
			$sql2 = "select distinct a.unicod as CODIGO, u.unidsc as DESCRICAO from acao a inner join unidade u on a.unicod=u.unicod where a.prgid=".$_SESSION['prgid']." order by u.unidsc";
			$db->monta_combo("unicod",$sql2,'S',"Todas as Unidades",$funcao_combo,'');
			?>
         </td>
    </tr>
    	<tr>
        <td align='right' class="subtitulodireita">Situação:</td>
         <td>
         <?
			$sql2 = "Select corcod as codigo,  corsignificadopar as descricao from cor order by corcod";
			$db->monta_combo("corcod",$sql2,'S',"Todas as Situações",$funcao_combo,'');
			?>
         </td>
    </tr>

<?if ($refcod) {?>
<?
//Parâmetros de Paginação
//Registros por página
$perpage = 50;
//Qtde de Paginas que serão mostradas na barrinha
$pages = 50;

//Montando a cláusula WHERE
$wh = "";
if ($unicod) $wh .= " and aca.unicod=".$unicod;
if ($corcod) $wh .= " and avp.corcod=".$corcod;
if ($corcod == '0') $wh .= " and avp.corcod is null";

//Query que traz as ações baseado no filtro escolhido
$sql = "select distinct aca.acaid, aca.acacod, aca.prgcod, aca.unicod, aca.loccod, aca.acadsc, aca.procod, avp.avptexto, case when usu.usunome is null then 'Não Atribuido' else usu.usunome end as usunome,
uni.unidsc, sacdsc, unm.unmdsc, pro.prodsc, case when cor.corcod is null then 0 else cor.corcod end as corcod , case when cor.corsignificadopar is null then 'Não Validado' else cor.corsignificadopar end as corsignificadopar,
tac.tacdsc
from acao aca
inner join unidade uni on aca.unicod = uni.unicod
inner join tipoacao tac on aca.taccod = tac.taccod
left join usuarioresponsabilidade usr on aca.acaid = usr.acaid and usr.pflcod=1 and rpustatus='A'
left join usuario usu on usr.usucpf = usu.usucpf
left join unidademedida unm on aca.unmcod = unm.unmcod
left join produto pro on aca.procod = pro.procod
left join avaliacaoparecer avp on aca.acaid=avp.acaid and tpaid=2 and refcod=".$refcod." 
left join cor on avp.corcod = cor.corcod
where aca.acastatus='A' and aca.acasnrap='f' and aca.prgid=".$_SESSION['prgid'].$wh."
 order by 3,2,4,5 ";

$RS = $db->carregar($sql);
$arrAcas = array();
if ($RS<>''){
foreach ($RS as $linha) {
	$arrAcas[] = $linha["acaid"];
}
$acaids = implode(",", $arrAcas);
$sql2 = "SELECT * FROM ex_fisico_financeiro WHERE acaid IN (" . $acaids . ") AND grefcod = " . $refcod;

$RS3 = $db->carregaAgrupado($sql2, "acaid");
}
if (is_array($RS)) {?>
</table>
<?
	$cont = 0;
	$nlinhas = count($RS);
	//Total de Registros 
	$total_reg = $nlinhas+1;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
	for ($i=($numero-1);$i<$reg_fim;$i++)  {
		foreach($RS[$i] as $k=>$v) ${$k}=$v;?>  
		<table id="tabelaacao<?=$cont?>" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:333333;<?=$largura?>;border-top: 2px solid #404040;">
		  <tr bgcolor="#ffffcc">
		    <td align='right' class="subtitulodireita" width="10%">Ação:</td>
		  	<td width="40%"><?if ($gerente) {?><input type="checkbox" name="acaid" id="<?=$acaid?>" value="<?=$acaid?>" onclick="retorna(<?=$cont?>);"><?}?><font color="#0F55A9"><?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod?></font> - <font color="#006600"><?=$acadsc?></font><input type="hidden" name="acadsc" value="<?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod.' - '.$acadsc?>"></td>
		  	<td align='right' class="subtitulodireita" width="10%"><b>Situação:</b></td>
		  	<td style="color:#000000" width="40%"><img src="imagens/valida<?=$corcod?>.gif" align="absmiddle" width="15" height="15"> <b><?=$corsignificadopar?></b></td>
		  </tr>
		  <tr>
		  	<td align='right' class="subtitulodireita">Unidade:</td><td><?=$unicod.' - '.$unidsc?></td>
		  	<td align='right' class="subtitulodireita">Localizador:</td><td><?=$loccod.' - '.$sacdsc?></td>
		  </tr>
		  <tr>
		  	<td align='right' class="subtitulodireita">Tipo:</td><td><?=$tacdsc?></td>
		  	<td align='right' class="subtitulodireita">Coordenador:</td><td><?=$usunome?></td>
		  	
		  </tr>
		  <tr>
		  	<td align='right' class="subtitulodireita">Produto:</td><td><?=$prodsc?></td>
		  	<td align='right' class="subtitulodireita">Unid. Medida:</td><td><?=$unmdsc?></td>
		  </tr>
		  	 	<tr id="texto<?=$cont?>" <?if ($gerente and $corcod<>3) {?> style="visibility:hidden;display:none;"<?}?>>
		  				<td colspan="7" id="td<?=$cont?>" align="left" style="padding-left:5px">Motivo da discordância sobre a Ação (<?=$acacod.'.'.$unicod.'.'.$loccod?>):<br><textarea disabled id="avptexto<?=$acaid?>" name="avptexto[<?=$acaid?>]" rows="3" class="normal" onmouseover="MouseOver(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="width:99%;"><?=$avptexto?></textarea></td>
		  			</tr> 
		  <tr>
		  	<td colspan="4" style="padding:0px;">
		  		<?//Mostra o executado físico/financeiro
				$sql2 = "select * from ex_fisico_financeiro where acaid=".$acaid." and grefcod=".$refcod;
				$RS2 = $RS3[$acaid];
		//		print 'rs2='.print_r($RS2); //exit();
				$nlinhas2 = count($RS2)-1;
				?>
		  		<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:333333;width:100%;">
		
		  			<tr bgcolor="#E3E3E3">
		  				<td>Execução Fisico-Financeira</td>
		  				<?for ($j=0; $j<=$nlinhas2;$j++) {?>
		  				<td align="right" style="color:#0F55A9;"><?=$RS2[$j]['refdsc']?></td>
		  				 <?}?>
		  				<td align="right"><b>Executado</b></td>
		  				<td align="right">Previsto na LOA</td>
		  				<td align="right">% Executado</td>
		  			</tr>
					<?//verifica se pode ser acompanhado (Se tem produto pode!) nao pode o procod 0,3
					if (!in($procod,array('0','3'))) {?>
		  			<tr>
		  				<td align="right">Físico<br><font color="#aaaaaa">(em <?=$unmdsc?>)</font></td>
		  				<?$totalfisrealizado = 0;
		  				for ($j=0; $j<=$nlinhas2;$j++) {?>
		  				<td title="<?=$RS2[$j]['corsignificado']?>" align="right" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';"><?=number_format($RS2[$j]['exprealizado'], 0, ',', '.');?><br><?
                          if ($RS2[$j]['avpid'])
                          {?>
                          <a href="javascript:visualiza('<?=md5_encrypt($RS2[$j]['avpid'],'')?>')">
                           <?}?>
                          <img border="0" width="25" height="10" src="imagens/<?=$RS2[$j]['corimgav']?>">
                          <? if ($RS2[$j]['avpid']) print '</a>'; ?>
                          </td>

		  				<?$totalfisrealizado = $totalfisrealizado+$RS2[$j]['exprealizado'];}?>
		  				<td align="right" valign="top"><?=number_format($totalfisrealizado, 0, ',', '.');?></td>
		  				<td align="right" valign="top"><?=number_format($RS2[0]['fisqtdeprevistoano'], 0, ',', '.');?></td>
		  				<td align="right" valign="top"><?if ($RS2[0]['fisqtdeprevistoano']==0) print 0; else print number_format($totalfisrealizado*100/$RS2[0]['fisqtdeprevistoano'], 2, ',', '.');?></td>
		  			</tr>
		  			<?}?>
		  			<tr bgcolor="White">
		  				<td align="right">Financeiro<br><font color="#aaaaaa">(em R$)</font></td>
		  				<?$totalfinrealizado = 0;
		  				 for ($j=0; $j<=$nlinhas2;$j++) {?>
		  				<td align="right"><?=number_format($RS2[$j]['exfrealizado'], 2, ',', '.');?></td>
		  				<?$totalfinrealizado = $totalfinrealizado+$RS2[$j]['exfrealizado'];}?>
		  				<td align="right"><?=number_format($totalfinrealizado, 2, ',', '.');?></td>
		  				<td align="right"><?=number_format($RS2[0]['finvlratualano'], 2, ',', '.');?></td>
		  				<td align="right"><?if ($RS2[0]['finvlratualano']==0) print 0; else print @number_format($totalfinrealizado*100/$RS2[0]['finvlratualano'], 2, ',', '.');?></td>
		  			</tr>
		  			<tr bgcolor="#E3E3E3">
		  				<td colspan="7" align="right" style="color:#008080">* Clique no dado físico para ver o detalhamento da execução.</td>
		  			</tr>
		  		</table>
		  	</td>
		  </tr>
		</table>
	<?$cont++;}
	
if ($gerente) {?>

</div>
<table id="tabelaacao<?=$cont?>" border="0" bgcolor="#F5F5F5" width="95%" cellpadding="2" align="center" style="border-right: 1px solid #cccccc;border-left: 1px solid #cccccc;border-bottom: 2px solid #cccccc;width:95%;">
	<tr>
	<td colspan="4"><select multiple size="6" name="acaidvalida[]" id="acaidvalida" style="width:100%;" class="CampoEstilo" onchange="moveto(this);">
	<option value="">-- Selecione as Ações na Listagem Acima...</option>
	</select></td>
	</tr>
	<tr>
	<?if ($i-$numero <= $perpage){?>
		<td><input type="button" name="Marcar Todas" value="Marcar Todas" onclick="marcatodas();"></td><?}?>
		<td><font color="#000099"><?=$nlinhas?> Ações no Total</font><br><font color="#006600"><?=$i-$numero+1?> Ações Listadas</font><br><font color="#990000"><span id="acamarcadas">0</span> Ações Marcadas</font></td>
		<td align="right"><input type="button" name="Validar com Concordância" value="Validar com Concordância" onclick="selectAllOptions(campoSelect);envia(1);"> <input type="button" name="Validar com Disccordância" value="Validar com Discordância" onclick="selectAllOptions(campoSelect);envia(3);"> <input type="button" name="Enviar para Pendência" value="Enviar para Pendência" onclick="selectAllOptions(campoSelect);envia(2);"></td>
	</tr>
	<tr><td colspan="3" align="right" bgcolor="#dcdcdc" style="padding:1px;"><?include "includes/paginacao.inc";?></td></tr>
</table>

<table id="tabelaincluir" border="0" bgcolor="#F5F5F5" width="95%" cellpadding="2" align="center" style="border-right: 1px solid #cccccc;border-left: 1px solid #cccccc;border-bottom: 2px solid #cccccc;visibility:hidden;display:none;">
	<tr>
	<td align="center">
	<input type="button" name="Incluir" value="Validar Ações com Discordância" onclick="discordar();">
	</td></tr>
</table>
<input type="hidden" name="acaid" value="">
<input type="hidden" name="acadsc" value="">
</form>
</center>
	<?} else {
		print "<center><div style='WIDTH:95%; border-bottom: 1px solid #cccccc;'>"; include "includes/paginacao.inc"; print "</div></center>";
		}
	}
 else {?>
	<tr>
		<td class="SubTituloDireita"></td>
		<td style="color:#990000">Não foram encontrados registros para sua pesquisa.</td>
	</tr>
<?}?>
<?}?>
	<?if (!$refcod or !is_array($RS)) {?>
    <tr>
		<td class="SubTituloDireita"></td>
		<td bgcolor="#dcdcdc"><input type="button" name="Listar" value="Listar Ações" onclick="escolhe_referencia()"></td>
	</tr>
</table>
</form>
</div>
</center>
<?}?>
<script language="JavaScript">


var campoSelect = document.formulario.refcod.options;
var obj = document.getElementById("referencia");
obj.firstChild.nodeValue='('+campoSelect[campoSelect.selectedIndex].text+')';
var campoSelect = document.getElementById("acaidvalida");
var campodsc = document.formulario.acadsc;
var campoid = document.formulario.acaid;
var campototal = document.getElementById("acamarcadas");
var qtdmarcadas = 0;

function retorna(objeto)
{

	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (document.formulario.acaid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(campodsc[objeto].value, campoid[objeto].value, false, false);
		sortSelect(campoSelect);
		qtdmarcadas ++;
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (campoid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;qtdmarcadas--;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('-- Selecione as Ações na Listagem Acima...', '', false, false); qtdmarcadas = 0;}
			sortSelect(campoSelect);
	}
	campototal.firstChild.nodeValue=qtdmarcadas;
}

function moveto(obj) {
		document.getElementById(obj.value).focus();
}

function marcatodas(){
	for(var i=0; i<=document.formulario.acaid.length-1; i++){
		if (document.formulario.acaid[i].checked == false && document.formulario.acaid[i].value!="") {document.formulario.acaid[i].checked = true;retorna(i)};
	}
}

   
	function visualiza(cod)
	{
		e = "simec.php?modulo=principal/ppafasequalitativa/acao/mostraavpar&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480'");
	}
    function envia_email(cpf)
    {
          e = "simec.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }

    function escolhe_referencia()
	{
		if (document.formulario.refcod.value=='') {alert('Selecione a Referência');document.formulario.refcod.focus();}
		else {document.formulario.submit();}
 	}
 	
 	
 	function discordar()
 	{
 				
 				branco='';
 				for(var i=0; i<=document.formulario.acaid.length-1; i++) {
						if (document.formulario.acaid[i].checked == true) 
						 { var campotexto = document.getElementById("avptexto"+document.formulario.acaid[i].value);
						 	
						 	if (campotexto.value == '') branco=document.formulario.acaid[i].value;}
 				}
 				if (branco) {alert('O Motivo da discordância não pode estar em branco!');document.getElementById("avptexto"+branco).focus();}
 				else
 				{
 					document.formulario.corcod_par.value=3;document.formulario.submit();
 				}
 	};
 	
 	function envia(act)
	{
		if (act==1) situaca = "CONCORDO";
		if (act==2) situaca = "PENDENTE";
		if (act==3) situaca = "DISCORDÂNCIA";
		
		if (campoSelect.options[0].value=='') {alert('Selecione as Ações a serem submetidas!');}
		else {
			if (act==3)
			{
				if (confirm('Ao validar as ações como '+situaca+', deverá ser informado o\nMotivo da discordância sobre a(s) '+qtdmarcadas+' Ação(ões) selecionadas.\nDeseja Continuar?')){
					for(var i=0; i<=document.formulario.acaid.length-1; i++) {
						if (document.formulario.acaid[i].checked == true) 
						{document.formulario.acaid[i].style.visibility = "hidden";
						document.formulario.acaid[i].style.display = "none";
						document.getElementById("texto"+i).style.visibility = "visible";
						document.getElementById("texto"+i).style.display = "";
						document.getElementById("texto"+i).style.background = "#ffffcc";
						document.getElementById("td"+i).style.color = "#cc0000";
						document.getElementById("avptexto"+document.formulario.acaid[i].value).disabled=0;}
						else 
						{document.getElementById("tabelaacao"+i).style.visibility = "hidden";document.getElementById("tabelaacao"+i).style.display = "none";}
					}
					document.getElementById("valida").style.height = '';
					document.getElementById("tabelaincluir").style.visibility = "visible";
					document.getElementById("tabelaincluir").style.display = "";
						
				}
			}
			else
			{
			if (confirm('Confirma a validação de  '+qtdmarcadas+' Ação(ões) como: '+situaca+'?')) {
				document.formulario.corcod_par.value=act;document.formulario.submit();
				}
			}
 		}
	}
 	
 	function procura()
	{
		if (document.formulario.prgcod=='') {alert('O Código do Programa não pode ser deixado em branco!');}
		else {document.formulario.numero.value='';document.formulario.submit();}
 	}
 	
 	function pagina(numero) {document.formulario.numero.value=numero;document.formulario.submit();}
</script>
