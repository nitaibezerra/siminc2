<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   MÃ³dulo:inclusao_usuario.INC
   Finalidade: permitir o controle de cadastro de usuários do simec
   */
	  include "includes/classes_simec.inc";
      include "includes/funcoes.inc";
	  $db = new cls_banco();
include "includes/erros.inc";
if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro)))
{
//print_r($_REQUEST);
  // checar consistência de cpf
  // se o cpf já existir, então, avisa e devolve.
  $sql = "select usucpf from usuario where usucpf = '".corrige_cpf($_REQUEST['usucpf'])."'";
  $usu = $db->recuperar($sql);

	if (is_array($usu)) {
	   // existe cpf idêntico, logo, tem que bloquear
	   ?>
	      <script>
	         alert ('O CPF: <?=$_REQUEST['usucpf']?> já se encontra cadastrado no sistema.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
        // obter os dados da instituição
        $sql = "select ittemail_inclusao_usuario, ittemail, itttelefone1, itttelefone2, ittddd, ittfax from instituicao where ittstatus = 'A'";
         $saida = $db->recuperar($sql);
        if(is_array($saida)) {
	         foreach($saida as $k=>$v) ${$k}=$v;}
   // fazer inserção de usuário na base de dados.
   $senha = senha();
   $_SESSION['usucpf'] = corrige_cpf($_REQUEST['usucpf']);
   $_SESSION['usucpforigem'] = corrige_cpf($_REQUEST['usucpf']);
   $sql = "insert into usuario (usucpf,usunome, usuemail, usustatus, usufoneddd, usufonenum, usufuncao, orgcod, unicod, usuchaveativacao,regcod,ususexo,usunivel,usuobs,ungcod,ususenha) values (".
   "'".corrige_cpf($_REQUEST['usucpf'])."',".
    "'".str_to_upper($_REQUEST['usunome'])."',".
   "'".$_REQUEST['usuemail']."',".
   "'X',".
   "'".$_REQUEST['usufoneddd']."',".
   "'".$_REQUEST['usufonenum']."',".
   "'".$_REQUEST['usufuncao']."',".
   "'".$_REQUEST['orgcod']."',".
   "'".$_REQUEST['unicod']."',".
   "'f',".
   "'".$_REQUEST['regcod']."',".
   "'".$_REQUEST['ususexo']."',".
   "'".$_REQUEST['usunivel']."',".
   "'".$_REQUEST['usuobs']."',".
   "'".$_REQUEST['ungcod']."',".
 "'".md5_encrypt($senha,'')."')";
 	//print $sql."<br>";
    $db->executar($sql);
   
   //Inclui Programas propostos
   $nlinhas = count($_REQUEST['usuprgproposto']);
   for ($i=0; $i<$nlinhas;$i++)
   {
   	 $campo = explode('.',$_REQUEST['usuprgproposto'][$i]);
	 $sql =  "insert into progacaoproposto (prgid, acacod, unicod, usucpf) values ($campo[0], '$campo[1]', '$campo[2]', '".$_SESSION['usucpf']."')";
	 //print $sql."<br>";
	 $db->executar($sql);
   }
   
   //Inclui Ações propostos
   $nlinhas = count($_REQUEST['usuacaproposto']);
   for ($i=0; $i<$nlinhas;$i++)
   {
   	 $campo = explode('.',$_REQUEST['usuacaproposto'][$i]);
	 $sql =  "insert into progacaoproposto (prgid, acacod, unicod, usucpf) values ($campo[0], '$campo[1]', '$campo[2]', '".$_SESSION['usucpf']."')";
	 //print $sql."<br>";
	 $db->executar($sql);
   }

   $db -> commit();
    $sql="select o.orgdsc,un.unidsc, g.ungdsc from usuario u left join orgao o on o.orgcod=u.orgcod left join unidade un on un.unicod=u.unicod left join unidadegestora g on g.ungcod=u.ungcod where u.usucpf = '".corrige_cpf($_REQUEST['usucpf'])."'";
    $registro=$db->recuperar($sql);
if ($registro['taccod'] == 1) $etapafase = 'Etapa'; else $etapafase='Fase';

     // envia email
        $assunto = 'Inscrição no cadastro do Simec';
				$sexo = 'Prezado Sr.  ';
				if ($_REQUEST['ususexo'] == 'F') $sexo = 'Prezada Sra. ';
        $mensagem = $sexo. strtoupper($_REQUEST['usunome']).',<br><br>'.$ittemail_inclusao_usuario.' '.$ittemail.' ou nos telefones:'.$ittddd.' - '.$itttelefone1.' ou '.$itttelefone2. ' Fax '.$ittfax.'<br><br>';
        email(strtoupper($_REQUEST['usunome']), $_REQUEST['usuemail'], $assunto, $mensagem);
        email('Administrador do SIMEC-UMA',$GLOBALS["email_sistema"],'Solicitação de cadastro','O usuário <br> CPF:'.corrige_cpf($_REQUEST['usucpf']).'  '.str_to_upper($_REQUEST['usunome']).'<br>E-mail:'.$_REQUEST['usuemail'].'<br>Telefone: '.$_REQUEST['usufoneddd'].'-'.$_REQUEST['usufonenum'].'<br>Órgão:'.$registro['orgdsc'].' / '.$registro['unidsc'].' / '.$registro['ungdsc'].'<br> Acaba de solicitar sua inclusão no cadastro do ' SIGLA_SISTEMA);
    $db->sucesso($modulo);
}

	$usunome = $_POST['usunome'];
	$usuemail =$_POST['usuemail'];
	$usufoneddd = $_POST['usufoneddd'];
	$usufonenum=$_POST['usufonenum'];
	$orgcod=$_POST['orgcod'];
	$usucpf = $_POST['usucpf'];
	$usufuncao = $_POST['usufuncao'];
	$unicod = $_POST['unicod'];
	$regcod = $_POST['regcod'];
	$ususexo = $_POST['ususexo'];
?>
<title>Cadastro de Usuários do Simec</title>
<script language="JavaScript" src="includes/funcoes.js"></script>
<body>
<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='includes/listagem.css'>
<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
<? include "cabecalho.php";

?>
<br>
<?
$titulo_modulo='Ficha de Solicitação de Cadastro de Usuários';
$subtitulo_modulo='Preencha os Dados Abaixo e clique no botão: "Enviar Solicitação".<br>'.obrigatorio().' Indica Campo Obrigatório.';
monta_titulo($titulo_modulo,$subtitulo_modulo);
?>
<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
<form method="POST" name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
      <tr>
        <td align='right' class="subtitulodireita">CPF:</td>
	<td>
		<? $habil='S';$obrig='S';?>
		<?=campo_texto('usucpf',$obrig,$habil,'',19,14,'###.###.###-##','');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Nome completo:</td>
        <td>
		<?=campo_texto('usunome','S','','',50,50,'','');?>
	    </td>
      </tr>
      <tr>
        <td align = 'right' class="subtitulodireita">Sexo:</td>
        <td>
                <input type="radio" name="ususexo" value="M" <?=($ususexo=='M'?"CHECKED":"")?>>  Masculino
                &nbsp;<input type="radio" name="ususexo" value="F" <?=($ususexo=='F'?"CHECKED":"")?>> Feminino
         <?=obrigatorio();?>
         </td>
       </tr>
      
      <tr>
        <td align = 'right' class="subtitulodireita">Orgão:</td>
        <td >
	<?$sql = "select orgcod as CODIGO,orgcod||' - '||orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,'S',"Selecione o órgão",'atualizaComboUnidade','');
	 print obrigatorio();?></td>
      </tr>
	<?if ($orgcod) {?>
      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Orçamentária (UO):</td>
         <td >
	<?
	  $sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$orgcod."' order by unidsc ";
	  $db->monta_combo("unicod",$sql,'S',"Selecione a Unidade Orçamentária",'atualizaComboUnidade','');
	   print obrigatorio();
	?>
	</td>
      </tr>
	  <?}?>
	  	  <?
	  if ($unicod == '26101' and $orgcod== CODIGO_ORGAO_SISTEMA) {?>

      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Gestora (UG):</td>
         <td >
	<?
	  $sql = "select ungcod as CODIGO,ungcod||' - '||ungdsc as DESCRICAO from unidadegestora where ungstatus='A' and unitpocod='U' and unicod ='".$unicod."' order by ungdsc ";
	  $db->monta_combo("ungcod",$sql,'S',"Selecione a Unidade Gestora",'','');
	   print obrigatorio();
	?>
	</td>
      </tr>
	  <?}?>
    <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">UF do órgão:</td>
        <td >
	<?
	  $sql = "select regcod as codigo, regcod||' - '||descricaouf as descricao from uf where codigoibgeuf is not null order by 2";
	  $db->monta_combo("regcod",$sql,'S',"Selecione a UF",'','');
	  print obrigatorio();
	?>
	</td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Telefone (DDD) + Telefone:</td>
        <td>
		<?=campo_texto('usufoneddd','','','',3,2,'##','');?>
		<?=campo_texto('usufonenum','S','','',18,15,'###-####|####-####','');?>
	</td>
      </tr>
      <tr >
        <td align = 'right' class="subtitulodireita">Seu E-Mail:</td>
        <td ><?=campo_texto('usuemail','S','','',50,100,'','');?></td>
      </tr>
      <tr >
        <td align = 'right' class="subtitulodireita">Confirme o Seu E-Mail:</td>
        <td ><?=campo_texto('usuemail_c','S','','',50,100,'','');?><br>
		<font color="#006666">Obs: O Campo E-Mail é para uso individual. <b>Não utilizar e-mails coletivos</b>. Utilizar PREFERENCIALMENTE e-mail funcional.</font></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Função/Cargo:</td>
        <td>
		<?=campo_texto('usufuncao','S','','',50,100,'','');?>
	    </td>
      </tr>
		<tr>
        <td align='right' class="subtitulodireita">Observações:</td>
        <td>
		<?=campo_textarea('usuobs','N','S','',100,3,'');?><br>
		<font color="#006666">Se desejar, informe acima Observações: Ex.: motivo do seu cadastramento, suas atribuições, etc...</font>
	    </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Perfil desejado:</td>
        <td >
           <select name="usunivel" class="CampoEstilo" onchange="seleciona_perfil();">
		   <option value="">Selecione o Perfil Desejado</option>
		   <!--<option value="1" <?=($usunivel=='1'?"SELECTED":"")?>> Ministro, Secretário ou Subsecretário</option>
           <option value="2" <?=($usunivel=='2'?"SELECTED":"")?>> Diretor ou Coordenador</option>-->
           <option value="3" <?=($usunivel=='3'?"SELECTED":"")?>> Gerente de Programa do PPA</option>
            <option value="4" <?=($usunivel=='4'?"SELECTED":"")?>> Gerente Executivo</option>
	       <option value="5" <?=($usunivel=='5'?"SELECTED":"")?>> Coordenador de Ação</option>
           <option value="6" <?=($usunivel=='6'?"SELECTED":"")?>> Equipe de apoio a Gerentes de Programa</option>
		   <option value="8" <?=($usunivel=='8'?"SELECTED":"")?>> Equipe de apoio a Coordenadores de Ação</option>
           <option value="7" <?=($usunivel=='7'?"SELECTED":"")?>> Acesso para Consultas</option>
		   </select> <?=obrigatorio();?><br>
		   <font color="#006666">Perfil desejado é um simples indicador para auxiliar o setor de Cadastramento atribuir seu perfil de acesso definitivo.</font>
        </td>
       </tr>
      <tr id="prg">
        <td align='right' class="subtitulodireita">Programa (s):</td>
        <td>
		<select multiple size="5" name="usuprgproposto[]" id="usuprgproposto" style="width:500px;" onclick="seleciona_prg();"  class="CampoEstilo">
<option value="">Clique Aqui para Selecionar o(s) Programa(s)</option></select><?=obrigatorio();?>
		<br>
		<font color="#006666">Indique acima o(s) Programa(s) sob sua responsabilidade como <b id="prgresp"></b>.</font>
	    </td>
      </tr>
      <tr id="aca">
        <td align='right' class="subtitulodireita">Ação (ões):</td>
        <td>
		<select multiple size="5" name="usuacaproposto[]" id="usuacaproposto" style="width:500px;" onclick="seleciona_aca();"  class="CampoEstilo">
<option value="">Clique Aqui para Selecionar a(s) Ação(ões)</option></select><?=obrigatorio();?><br>
		<font color="#006666">Indique acima a(s) Ação(ões) sob sua responsabilidade como <b id="acaresp"></b>.</font>
	    </td>
      </tr>
      
<tr bgcolor="#C0C0C0">
 <td></td>
   <td><input type="button" name="btinserir" value="Enviar Solicitação"  onclick="validar_cadastro('I')">&nbsp;&nbsp;&nbsp;<input type="Button" value="Voltar" onclick="history.back();"></td>
 </tr>
</form>
 </table>
 <br>
 
<? include "rodape.php";?>
<script>
var selecionaprg = null;
document.getElementById("prg").style.visibility = "hidden";
document.getElementById("prg").style.display = "none";
document.getElementById("aca").style.visibility = "hidden";
document.getElementById("aca").style.display = "none";

function retorno(prgaca)
{
j = 0;
if (prgaca=="prg"){campoSelect = document.getElementById("usuprgproposto");} else {campoSelect = document.getElementById("usuacaproposto");}
	
tamanho = campoSelect.options.length-1;
	
	for(var i=tamanho; i>=0; i--)
		{
		campoSelect.options[i] = null;
		}

	for(var i=0; i<selecionaprg.document.formulario.prgid.length; i++)
		{if(selecionaprg.document.formulario.prgid[i].checked == true)
			{campoSelect.options[j] = new Option(selecionaprg.document.formulario.prgdsc[i].value, selecionaprg.document.formulario.prgid[i].value, false, false);
			j++;}
		}
	if (j == 0)
		{ if (prgaca=="prg") {campoSelect.options[j] = new Option('Clique Aqui para Selecionar o(s) Programa(s)', '', false, false);} else {campoSelect.options[j] = new Option('Clique Aqui para Selecionar a(s) Ação(ões)', '', false, false);} }
}

function retorna(objeto,prgaca)
{
	if (prgaca=="prg"){campoSelect = document.getElementById("usuprgproposto");} else {campoSelect = document.getElementById("usuacaproposto");}
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (selecionaprg.document.formulario.prgid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(selecionaprg.document.formulario.prgdsc[objeto].value, selecionaprg.document.formulario.prgid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (selecionaprg.document.formulario.prgid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){if (prgaca=="prg") {campoSelect.options[0] = new Option('Clique Aqui para Selecionar o(s) Programa(s)', '', false, false);} else {campoSelect.options[0] = new Option('Clique Aqui para Selecionar a(s) Ação(ões)', '', false, false);}}
			sortSelect(campoSelect);
	}
}	

function seleciona_prg()
{
	document.getElementById("usuprgproposto").selectedIndex = -1;
	selecionaprg = window.open("geral/seleciona_prg.php?campo=usuprgproposto", "selecionaprg","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480");
}

function seleciona_aca()
{
	document.getElementById("usuacaproposto").selectedIndex = -1;
	selecionaprg = window.open("geral/seleciona_aca.php?campo=usuacaproposto", "selecionaprg","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480");
}

function seleciona_perfil()
{
	//document.formulario.usuprgproposto.value = "";
	//document.formulario.usuacaproposto.value = "";
	document.getElementById("prg").style.visibility = "hidden";
	document.getElementById("prg").style.display = "none";
	document.getElementById("aca").style.visibility = "hidden";
	document.getElementById("aca").style.display = "none";
	
	campoSelect = document.getElementById("usuprgproposto");tamanho = campoSelect.options.length-1; for(var i=tamanho; i>=0; i--){campoSelect.options[i] = null;}campoSelect.options[0] = new Option('Clique Aqui para Selecionar o(s) Programa(s)', '', false, false);
	campoSelect = document.getElementById("usuacaproposto");tamanho = campoSelect.options.length-1; for(var i=tamanho; i>=0; i--){campoSelect.options[i] = null;}campoSelect.options[0] = new Option('Clique Aqui para Selecionar a(s) Ações(s)', '', false, false);
	aca_obrg = 0;
	prg_obrg = 0;

	if(document.all){
			document.getElementById("prgresp").innerText = "";}
		else {
			document.getElementById("prgresp").textContent = "";}
	if(document.all){
			document.getElementById("acaresp").innerText = "";}
		else {
			document.getElementById("acaresp").textContent = "";}

	//Gerente de Programa
	if (document.formulario.usunivel.value == 3)
		{
		prg_obrg = 1;
		if(document.all){
			document.getElementById("prgresp").innerText = "Gerente de Programa";}
		else {
			document.getElementById("prgresp").textContent = "Gerente de Programa";}
		
		document.getElementById("prg").style.visibility = "visible";
		document.getElementById("prg").style.display = "";
		}
	
	//Gerente Executivo
	if (document.formulario.usunivel.value == 4)
		{
		prg_obrg = 1;
		if(document.all){
			document.getElementById("prgresp").innerText = "Gerente Executivo";}
		else {
			document.getElementById("prgresp").textContent = "Gerente Executivo";}
		
		document.getElementById("prg").style.visibility = "visible";
		document.getElementById("prg").style.display = "";
		
		aca_obrg = 1;
		if(document.all){
			document.getElementById("acaresp").innerText = "Gerente Executivo";}
		else {
			document.getElementById("acaresp").textContent = "Gerente Executivo";}
		
		document.getElementById("aca").style.visibility = "visible";
		document.getElementById("aca").style.display = "";
		}
		
	//Coordenador de Ação
	if (document.formulario.usunivel.value == 5)
		{
		aca_obrg = 1;
		if(document.all){
			document.getElementById("acaresp").innerText = "Coordenador de Ação";}
		else {
			document.getElementById("acaresp").textContent = "Coordenador de Ação";}
		
		document.getElementById("aca").style.visibility = "visible";
		document.getElementById("aca").style.display = "";
		}

	//Equipe de Apoio Programa
	if (document.formulario.usunivel.value == 6)
		{
		prg_obrg = 1;
		if(document.all){
			document.getElementById("prgresp").innerText = "Equipe de Apoio a Gerentes de Programa";}
		else {
			document.getElementById("prgresp").textContent = "Equipe de Apoio a Gerentes de Programa";}
		
		document.getElementById("prg").style.visibility = "visible";
		document.getElementById("prg").style.display = "";
		}
		
	//Equipe de Apoio Ação
	if (document.formulario.usunivel.value == 8)
		{
		aca_obrg = 1;
		if(document.all){
			document.getElementById("acaresp").innerText = "Equipe de apoio a Coordenadores de Ação";}
		else {
			document.getElementById("acaresp").textContent = "Equipe de apoio a Coordenadores de Ação";}
		
		document.getElementById("aca").style.visibility = "visible";
		document.getElementById("aca").style.display = "";
		}

}

    function atualizaComboUnidade(cod) {
	 document.formulario.submit();
    }
	
    function validar_cadastro(cod) {
		if (!validaBranco(document.formulario.usucpf, 'CPF')) return;
		if (! DvCpfOk(document.formulario.usucpf))
		{
		    document.formulario.usucpf.focus();
		    return;
		}
		if (!validaBranco(document.formulario.usunome, 'Nome')) return;
		if (!validaRadio(document.formulario.ususexo,'Sexo')) return;
		if (!validaBranco(document.formulario.regcod, 'UF')) return;
		if (!validaBranco(document.formulario.orgcod, 'Órgão')) return;
		if (document.formulario.unicod.options[1].value){if (!validaBranco(document.formulario.unicod, 'Unidade Orçamentária (UO)')) return;}
		if (document.formulario.ungcod){if (!validaBranco(document.formulario.ungcod, 'Unidade Gestora (UG)')) return;}
		if (!validaBranco(document.formulario.usufoneddd, 'DDD')) return;
		if (!validaBranco(document.formulario.usufonenum, 'Telefone')) return;
		if (!validaBranco(document.formulario.usuemail, 'Email')) return;
		if (document.formulario.usuemail_c.value != document.formulario.usuemail.value)
        {
            alert ("A confirmação do E-mail não coincide!. Verifique o E-mail.");
            document.formulario.usuemail.setfocus();
            return;
        }
		
		if(! validaEmail(document.formulario.usuemail.value))
		{
			alert("Email Inválido.");
			document.formulario.usuemail.focus();
			return;
		}
       	if (!validaBranco(document.formulario.usufuncao, 'Função/Cargo')) return;
	   	if (!validaBranco(document.formulario.usunivel,'Perfil Desejado')) return;
	   	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
		if (prg_obrg == 1) {if (document.getElementById("usuprgproposto").options[0].value=='') {alert('Campo Obrigatório: Programa(s)'); return;} else {selectAllOptions(document.getElementById("usuprgproposto"));}}
		if (aca_obrg == 1) {if (document.getElementById("usuacaproposto").options[0].value=='') {alert('Campo Obrigatório: Ação(ões)'); return;} else {selectAllOptions(document.getElementById("usuacaproposto"));}}
		document.formulario.submit();
     }
	 
	 function sortSelect(obj) {
			var o = new Array();
			if (obj.options==null) { return; }
			for (var i=0; i<obj.options.length; i++) {
				o[o.length] = new Option( obj.options[i].text, obj.options[i].value, obj.options[i].defaultSelected, obj.options[i].selected) ;
				}
			if (o.length==0) { return; }
			o = o.sort( 
				function(a,b) { 
					if ((a.text+"") < (b.text+"")) { return -1; }
					if ((a.text+"") > (b.text+"")) { return 1; }
					return 0;
					} 
				);
		
			for (var i=0; i<o.length; i++) {
				obj.options[i] = new Option(o[i].text, o[i].value, o[i].defaultSelected, o[i].selected);
				}
	}
	
	function selectAllOptions(obj) {
	for (var i=0; i<obj.options.length; i++) {
		obj.options[i].selected = true;
		}
	}
</script>
</body>
</html>
