<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:CADUSUARIO.INC
   Finalidade: permitir o controle de cadastro de usuários do simec
 */

$modulo=$_REQUEST['modulo'] ;//
$encontrou = false;

if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro)))
{
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
   $sql = "insert into usuario (usucpf,usunome,usuemail,usustatus, usufoneddd,usufonenum,usufuncao,orgcod,unicod,usuchaveativacao,regcod,ususexo,ungcod,ususenha) values (".
   "'".corrige_cpf($_REQUEST['usucpf'])."',".
    "'".str_to_upper($_REQUEST['usunome'])."',".
   "'".$_REQUEST['usuemail']."',".
   "'I',".
   "'".$_REQUEST['usufoneddd']."',".
   "'".$_REQUEST['usufonenum']."',".
   "'".$_REQUEST['usufuncao']."',".
   "'".$_REQUEST['orgcod']."',".
   "'".$_REQUEST['unicod']."',".
   "'f',".
   "'".$_REQUEST['regcod']."',".
   "'".$_REQUEST['ususexo']."',".
   "'".$_REQUEST['ungcod']."',".
 "'".md5_encrypt($senha,'')."')";
   $saida = $db->executar($sql);
   $db -> commit();
      $_SESSION['usuario']=$_REQUEST['usucpf'];
     // envia email
        $assunto = 'Inscrição no cadastro do Simec';
				$sexo = 'Prezado Sr.  ';
				if ($_REQUEST['ususexo'] == 'F') $sexo = 'Prezada Sra. ';
        $mensagem = $sexo. str_to_upper($_REQUEST['usunome']).chr(13).$ittemail_inclusao_usuario.' '.$ittemail.' ou nos telefones:'.$ittddd.' - '.$itttelefone1.' ou '.$itttelefone2. ' Fax '.$ittfax.chr(13).'Senha = '.$senha;
        email(str_to_upper($_REQUEST['usunome']), $_REQUEST['usuemail'], $assunto, $mensagem);
    $db->sucesso($modulo);

}
if ($_REQUEST['act']=='alterar')
{

   // fazer alteração de usuario na base de dados.
   $sql = "update usuario set usunome='".str_to_upper($_REQUEST['usunome'])."',".
   "usuemail='".$_REQUEST['usuemail']."',".
   "usufoneddd='".$_REQUEST['usufoneddd']."',".
   "usufonenum='".$_REQUEST['usufonenum']."',".
   "usufuncao='".$_REQUEST['usufuncao']."',".
   "orgcod='".$_REQUEST['orgcod']."',".
   "unicod='".$_REQUEST['unicod']."',".
   "regcod='".$_REQUEST['regcod']."',".
   "ungcod='".$_REQUEST['ungcod']."',".
   "usuobs='".$_REQUEST['usuobs']."',".
   "ususexo='".$_REQUEST['ususexo']."'".
   " where usucpf='".$_REQUEST['usucpf']."'";
      $sql2 = "update seguranca.usuario_sistema set susstatus='".$_REQUEST['usustatus']."' where usucpf='".$_REQUEST['usucpf']."' and sisid=".$_SESSION['sisid'];

      //	dbg($sql,1);
   if ($_REQUEST['status']=='X' and $_REQUEST['usustatus']=='A') {
     // passou da situação de aguardando aprovação para aprovado, então envia email
           $sql3= "select usustatus from seguranca.usuario where usucpf='".$_REQUEST['usucpf']."'";
	  $stat = $db->pegaum($sql3);
	 if ($stat <> 'A')  $senha = senha();	 else $senha=0;
	 if ($senha){
      $sql = "update usuario set usunome='".str_to_upper($_REQUEST['usunome'])."',".
   "usuemail='".$_REQUEST['usuemail']."',".
   "usustatus='A',".
   "usufoneddd='".$_REQUEST['usufoneddd']."',".
   "usufonenum='".$_REQUEST['usufonenum']."',".
   "usufuncao='".$_REQUEST['usufuncao']."',".
   "orgcod='".$_REQUEST['orgcod']."',".
   "unicod='".$_REQUEST['unicod']."',".
   "regcod='".$_REQUEST['regcod']."',".
   "usuobs='".$_REQUEST['usuobs']."',".
   "ususexo='".$_REQUEST['ususexo']."',".
   "usudataultacesso=now(),".   
    "usutentativas=0,".
   "ususenha='".md5_encrypt($senha,'')."'".
   " where usucpf='".$_REQUEST['usucpf']."'";
	  }
	  else
	   {
	   	$sql = "update usuario set usunome='".str_to_upper($_REQUEST['usunome'])."',".
   "usuemail='".$_REQUEST['usuemail']."',".
   "usustatus='A',".
   "usufoneddd='".$_REQUEST['usufoneddd']."',".
   "usufonenum='".$_REQUEST['usufonenum']."',".
   "usufuncao='".$_REQUEST['usufuncao']."',".
   "orgcod='".$_REQUEST['orgcod']."',".
   "unicod='".$_REQUEST['unicod']."',".
   "regcod='".$_REQUEST['regcod']."',".
   "usuobs='".$_REQUEST['usuobs']."',".
   "ususexo='".$_REQUEST['ususexo']."',".
   "usudataultacesso=now(),".   
    "usutentativas=0".
   " where usucpf='".$_REQUEST['usucpf']."'";
	   }
	      $sql2 = "update seguranca.usuario_sistema set susstatus='A' where usucpf='".$_REQUEST['usucpf']."' and sisid=".$_SESSION['sisid'];
   }
   
  
     if ($_REQUEST['status']=='I' and $_REQUEST['usustatus']=='A') {
     // passou da situação de inativo para aprovado não envia e-mail
    //$senha = senha();
      $sql = "update usuario set usunome='".str_to_upper($_REQUEST['usunome'])."',".
   "usuemail='".$_REQUEST['usuemail']."',".
   "usustatus='".$_REQUEST['usustatus']."',".
   "usufoneddd='".$_REQUEST['usufoneddd']."',".
   "usufonenum='".$_REQUEST['usufonenum']."',".
   "usufuncao='".$_REQUEST['usufuncao']."',".
   "orgcod='".$_REQUEST['orgcod']."',".
   "unicod='".$_REQUEST['unicod']."',".
   "regcod='".$_REQUEST['regcod']."',".
   "usudataultacesso=now(),".
   "usuobs='".$_REQUEST['usuobs']."',".
   "ususexo='".$_REQUEST['ususexo']."',".
 // "usuchaveativacao='f',".
  "usutentativas=0".
  // "ususenha='".md5_encrypt($senha,'')."'".
   " where usucpf='".$_REQUEST['usucpf']."'";
      $sql2 = "update seguranca.usuario_sistema set susstatus='A' where usucpf='".$_REQUEST['usucpf']."' and sisid=".$_SESSION['sisid'];
  }
  
  
   if ($_REQUEST['status']=='C' and $_REQUEST['usustatus']=='A') {
     // passou da situação de cancelado para aprovado, então envia email
	       $sql3= "select usustatus from seguranca.usuario where usucpf='".$_REQUEST['usucpf']."'";
	  $stat = $db->pegaum($sql3);
	  if ($stat <> 'A')  $senha = senha();	 else $senha=0;
	   if ($senha) {
      $sql = "update usuario set usunome='".str_to_upper($_REQUEST['usunome'])."',".
   "usuemail='".$_REQUEST['usuemail']."',".
   "usustatus='A',".
   "usufoneddd='".$_REQUEST['usufoneddd']."',".
   "usufonenum='".$_REQUEST['usufonenum']."',".
   "usufuncao='".$_REQUEST['usufuncao']."',".
   "orgcod='".$_REQUEST['orgcod']."',".
   "unicod='".$_REQUEST['unicod']."',".
   "regcod='".$_REQUEST['regcod']."',".
   "usuobs='".$_REQUEST['usuobs']."',".
   "ususexo='".$_REQUEST['ususexo']."',".
    "usudataultacesso=now(),".   
  "usuchaveativacao='f',".
  "usutentativas=0,".
   "ususenha='".md5_encrypt($senha,'')."'".
   " where usucpf='".$_REQUEST['usucpf']."'";
	   }
	   else
	   {
	   	      $sql = "update usuario set usunome='".str_to_upper($_REQUEST['usunome'])."',".
   "usuemail='".$_REQUEST['usuemail']."',".
   "usustatus='A',".
   "usufoneddd='".$_REQUEST['usufoneddd']."',".
   "usufonenum='".$_REQUEST['usufonenum']."',".
   "usufuncao='".$_REQUEST['usufuncao']."',".
   "orgcod='".$_REQUEST['orgcod']."',".
   "unicod='".$_REQUEST['unicod']."',".
   "regcod='".$_REQUEST['regcod']."',".
   "usuobs='".$_REQUEST['usuobs']."',".
   "ususexo='".$_REQUEST['ususexo']."',".
   "usudataultacesso=now(),".   
  "usutentativas=0".
   " where usucpf='".$_REQUEST['usucpf']."'";
	   }
	   $sql2 = "update seguranca.usuario_sistema set susstatus='A' where usucpf='".$_REQUEST['usucpf']."' and sisid=".$_SESSION['sisid'];
  }	
	   	
	   	
	   	
   if ($_REQUEST['cbsenha']) {
     // cria a senha padrão
     $senha = 'simecspo';
     $sql = "update usuario set usunome='".str_to_upper($_REQUEST['usunome'])."',".
   "usuemail='".$_REQUEST['usuemail']."',".
   "usustatus='".$_REQUEST['usustatus']."',".
   "usufoneddd='".$_REQUEST['usufoneddd']."',".
   "usufonenum='".$_REQUEST['usufonenum']."',".
   "usufuncao='".$_REQUEST['usufuncao']."',".
   "orgcod='".$_REQUEST['orgcod']."',".
   "unicod='".$_REQUEST['unicod']."',".
   "regcod='".$_REQUEST['regcod']."',".
   "usuobs='".$_REQUEST['usuobs']."',".
   "ususexo='".$_REQUEST['ususexo']."',".
   "usudataultacesso=now(),".   
  "usuchaveativacao='f',".
  "usutentativas=0,".
   "ususenha='".md5_encrypt($senha,'')."'".
   " where usucpf='".$_REQUEST['usucpf']."'";
      $sql2 = "update seguranca.usuario_sistema set susstatus='A' where usucpf='".$_REQUEST['usucpf']."' and sisid=".$_SESSION['sisid'];
  }
   $saida = $db->executar($sql);
      $saida = $db->executar($sql2);
   $db -> commit();
   
   $_SESSION['usuario']=$_REQUEST['usucpf'];
   if ($_REQUEST['status']=='X' and $_REQUEST['usustatus']=='A')
   {
      $assunto = 'Aprovação de inscrição no cadastro do Simec - Módulo de Proposta Orçamentária';
	  $sexo = 'Prezado Sr.  ';
	  if ($_REQUEST['ususexo'] == 'F') $sexo = 'Prezada Sra. ';
	  if ($senha)
      $mensagem = $sexo. strtoupper($_REQUEST['usunome']).chr(13)."<br>Sua solicitação de cadastro no Módulo de Proposta Orçamentária foi aprovada.<br> Sua Senha de acesso é:".$senha;
      else 
      $mensagem = $sexo. strtoupper($_REQUEST['usunome']).chr(13)."<br>Sua solicitação de cadastro no Módulo de Proposta Orçamentária foi aprovada.";
      
      email(strtoupper($_REQUEST['usunome']), $_REQUEST['usuemail'], $assunto, $mensagem);
   }
   if ($_REQUEST['status']=='C' and $_REQUEST['usustatus']=='A')
   {
      $assunto = 'Liberação de acesso no Simec - Módulo de Proposta Orçamentária';
	  $sexo = 'Prezado Sr.  ';
	  if ($_REQUEST['ususexo'] == 'F') $sexo = 'Prezada Sra. ';
	  if ($senha)
      $mensagem = $sexo. strtoupper($_REQUEST['usunome']).chr(13)."<br>Seu acesso ao Simec no Módulo de Proposta Orçamentária foi restabelecido. <br> Sua nova senha de acesso é:".$senha;
      else 
            $mensagem = $sexo. strtoupper($_REQUEST['usunome']).chr(13)."<br>Seu acesso ao Simec no Módulo de Proposta Orçamentária foi restabelecido." ;
      email(strtoupper($_REQUEST['usunome']), $_REQUEST['usuemail'], $assunto, $mensagem);
   }
   
   $db->sucesso($modulo);
}

include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$titulo_modulo='Cadastro de Usuários do Simec';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

if(($_REQUEST["usucpf"] or $_SESSION['usuario']) and !$_REQUEST["usucpf_int"]){ //}and $_REQUEST["acao"] in ('A','C') ) {
   if (! $_REQUEST["usucpf"]) $_REQUEST["usucpf"] =$_SESSION['usuario'];
       $sql= "select u.*, us.susstatus from seguranca.usuario u inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.sisid=".$_SESSION['sisid']." where u.usucpf='".$_REQUEST['usucpf']."'";

        $saida = $db->recuperar($sql);
        if(is_array($saida)) {
	   foreach($saida as $k=>$v) ${$k}=$v;
	   $encontrou = true;
    }
?>
<? } else {
	 //recupera todas as variaveis que veio pelo post
	 foreach($_POST as $k=>$v) ${$k}=$v;
}
?>
   <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<form method="POST" name="formulario">
	<input type=hidden name="modulo" value="<?=$modulo?>">
	<input type=hidden name="act" value=0>
	<input type=hidden name="status" value="<?=$susstatus?>">
	<input type='hidden' name="usucpf_int" value="<?=$usucpf?>">
      <tr>
        <td align='right' class="SubTituloDireita">CPF:</td>
	<td>
		<? if (! $encontrou or $_REQUEST['acao']<>'C')  {$habil='S';$obrig='S';} else {$habil= 'N';$obrig='N';}?>
		<?=campo_texto('usucpf',$obrig,'N','',19,14,'###.###.###-##','');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Nome completo:</td>
        <td>
		<?=campo_texto('usunome','S',$habil,'',50,50,'','');?>
	    </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Sexo:</td>
        <td>
                <input type="radio" name="ususexo" value="M" <?=($ususexo=='M'?"CHECKED":"")?>>  Masculino
                &nbsp;<input type="radio" name="ususexo" value="F" <?=($ususexo=='F'?"CHECKED":"")?>> Feminino
         <?=obrigatorio();?>
         </td>
       </tr>
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">UF:</td>
        <td >
	<?
	  $sql = "select regcod as CODIGO,regdsc as DESCRICAO from regiao order by regdsc ";
	  $db->monta_combo("regcod",$sql,$habil,"Selecione a UF",'','');
	?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão:</td>
        <td >
	<?$sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,$habil,"Selecione o órgão",'atualizaComboUnidade','');
	?><?=obrigatorio();?></td>
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
	      $org = substr($_SESSION['ittorgao'],0,2);
	  if ($unicod == $org.'101' and $orgcod==$_SESSION['ittorgao']) {?>
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
      <tr>
        <td align='right' class="SubTituloDireita">Telefone (DDD + Telefone):</td>
        <td>
		<?=campo_texto('usufoneddd','',$habil,'',5,2,'##','');?>|
		<?=campo_texto('usufonenum','',$habil,'',18,15,'###-####|####-####','');?>
	</td>
      </tr>
      <tr >
        <td align='right' class="SubTituloDireita">E-Mail:</td>
        <td ><?=campo_texto('usuemail','S',$habil,'',50,50,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Função/Cargo:</td>
        <td>
		<?=campo_texto('usufuncao','S',$habil,'',50,50,'','');?>
	    </td>
      </tr>
	<tr>
        <td align='right' class="subtitulodireita">Observações:</td>
        <td>
		<?=campo_textarea('usuobs','N','S','',100,3,'');?><br>
		<font color="#006666">Se desejar, informe acima Observações: Ex.: motivo do seu cadastramento, suas atribuições, etc...</font>
	    </td>
      </tr>
     <?
     $codigo = $_SESSION['sisid'];
     $_REQUEST["usucpf"]=formatar_cpf($_REQUEST["usucpf"]);
     include (APPRAIZ."/elabrev/modulos/sistema/usuario/incusuario.inc"); ?> 
	  <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Perfis associados:</td>
        <td><table border="1" cellpadding="2" cellspacing="0" width="500" class="listagem" bgcolor="#fefefe">
<?
///////////////////////////////////////////////////////////////////////////////
// Gerenciamento de responsabilidades										 //
//																			 //
// Desenvolvido por Adonias Malosso - <>				                     //
//                  Cristiano Cabral - <>		                          	 //
//                  Gilberto Xavier - <>		                          	 //
///////////////////////////////////////////////////////////////////////////////
$sql = "SELECT * FROM elabrev.tiporesponsabilidade WHERE tprsnvisivelperfil = 't' and tprsigla = 'U' ORDER BY tprdsc";
$responsabilidades = $db->carregar($sql);


$sqlPerfisUsuario = "SELECT p.pflcod, p.pfldsc FROM perfil p INNER JOIN perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = '%s' and sisid=".$_SESSION['sisid']." where p.pflstatus='A' ORDER BY p.pfldsc";

$query = sprintf($sqlPerfisUsuario, $usucpf);
$perfisUsuario = $db->carregar($query);
if($perfisUsuario && @count($perfisUsuario)>0) {
	?>
	<tr>
	  <td width="12" rowspan="2" bgcolor="#e9e9e9" align="center">&nbsp;</td>
	  <td rowspan="2" align="left" bgcolor="#e9e9e9" align="center">Descrição</td>
	  <td align="center" colspan="3" colspan="<?=@count($responsabilidades)?>" bgcolor="#e9e9e9" align="center">Responsabilidades</td>
	</tr>
	<tr>
		<td align="center" bgcolor="#e9e9e9" align="center">&nbsp;</td>
	<? foreach ($responsabilidades as $responsabilidade) {
		?><td align="center" colspan="2" bgcolor="#e9e9e9" align="center"><?=$responsabilidade["tprdsc"]?></td><?
	} ?>
	</tr>
	<?
	foreach ($perfisUsuario as $perfil) {
		$marcado = $i++ % 2 ? '#F7F7F7' : '';
		$sqlResponsabilidadesPerfil = "SELECT p.*, tr.tprdsc, tr.tprsigla 
			FROM (SELECT * FROM elabrev.tprperfil WHERE pflcod = '%s') p
			RIGHT JOIN elabrev.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
			WHERE tprsnvisivelperfil = TRUE ORDER BY tr.tprdsc";

		$query = sprintf($sqlResponsabilidadesPerfil, $perfil["pflcod"]);
		$responsabilidadesPerfil = $db->carregar($query);

		//
		// Esconde a imagem + para perfis sem responsabilidades
		$mostraMais = false;
		foreach ($responsabilidadesPerfil as $resPerfil) {
			if ((bool)$resPerfil["tprcod"]) { $mostraMais = true; break; }
		}
		
		?>
		<tr bgcolor="<?=$marcado?>">
		  <td style="color:#003c7b">
		  <? if ($mostraMais) { ?>
		    <a href="Javascript:abreconteudo('geral/listaresponsabilidades.php?usucpf=<?=$usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')">
			  <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$perfil["pflcod"]?>"/>
			</a>
		  <? } ?>
		  </td>
		  <td><?=$perfil["pfldsc"]?></td>
		<? foreach ($responsabilidadesPerfil as $resPerfil) {
			?><td align="center"><?
			if ((bool)$resPerfil["tprcod"]) {
				?><input type="button" name="btnAbrirResp<?=$perfil["pflcod"]?>" value="Atribuir" onclick="popresp(<?=$perfil["pflcod"]?>, '<?=$resPerfil["tprsigla"]?>')"><?
			}
			?></td><?			
		}
		 ?>
		</tr>
		<tr bgcolor="<?=$marcado?>">
		  <td colspan="10" id="td<?=$perfil["pflcod"]?>"></td>
		</tr>		
		<?
	}
}
else {
	?><tr>
	   <td></td><td>
	    <font color="red">Usuário sem perfil associado</font>
	  </td>
	</tr><?
}
?>
<tr><td></td><td colspan="10"><input type="button" name="perfilusu" value="Associar Perfil" onclick="associa_perfil()"></td></tr>
        	</table>
        </td>
</tr>

<tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Status:</td>
        <td>
        
            <input type="radio" id="usustatus_A" name="usustatus" value="A" <?if ($susstatus=='A') print "checked";?>> Ativo
            <input type="radio" name="usustatus" value="I" <?if ($susstatus=='I') print "checked";?>> Inativo
            <input type="radio" name="usustatus" value="C" <?if ($susstatus=='C') print "checked";?>> Cancelado
            <input type="radio" name="usustatus" value="X" <?if ($susstatus=='X') print "checked";?>> Aguardando aprovação
        </td>
</tr>

<? if (($db->testa_cgo() or $db->testa_superuser()) and $usustatus=='A'){?>
<tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Gerar senha padrão (simecspo):</td>
        <td><input type="checkbox" name="cbsenha"> </td>
</tr>
<?}?>
<?

if ($_REQUEST['acao']=='A') {?>
<tr bgcolor="#C0C0C0">
   <td></td>
   <td><input type="button" class="botao" name="btalterar" value="Atualizar" onclick="validar_cadastro('A')"><input type="button" class="botao" name="btvoltar" value="Voltar" onclick="history.back();"></td>
 </tr>
<? } else if ($_REQUEST['acao']=='I'){ ?>
<tr bgcolor="#C0C0C0">
 <td></td>
   <td><input type="button" class="botao" name="btinserir" value="Incluir"  onclick="validar_cadastro('I')"></td>

 </tr>
<? } ?>
</form>
 </table>

<script>
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */

  function associa_perfil()
  {
    if (document.formulario.usucpf.value != '')
    {
    WindowObjectReference = window.open("<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/usuario/associa_usuario&acao=A&usucpf=<?=$usucpf?>", "Associar_Perfil","menubar=no,location=no,resizable=no,scrollbars=yes,status=no,width=400,height=480");
    }
  }

  function popresp(pflcod, tprsigla)
  {
	switch(tprsigla) {
		case 'A':
			abreresp = window.open("<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/seleciona_aca_perfilresp&acao=A&pflcod="+pflcod+"&usucpf=<?=$usucpf?>", "popresp","menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=500,height=480");		
		break;
		case 'P':
			abreresp = window.open("geral/seleciona_prg_perfilresp.php?pflcod="+pflcod+"&usucpf=<?=$usucpf?>", "popresp","menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=500,height=480");		
		break;
		case 'U':
			abreresp = window.open("geral/seleciona_unid_perfilresp.php?pflcod="+pflcod+"&usucpf=<?=$usucpf?>", "popresp","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=520,height=480");		
		break;		
	}
  }
  
  function atualizaComboUnidade(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
    function ProcuraUsuario() {
	 if (document.formulario.usunome)
	 {
	    document.formulario.act.value = "procura";
	    document.formulario.submit()
	 }
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
		if (!validaBranco(document.formulario.orgcod, 'Órgão')) return;
		if (!validaBranco(document.formulario.usuemail, 'Email')) return;
		if(! validaEmail(document.formulario.usuemail.value))
		{
			alert("Email Inválido.");
			document.formulario.usuemail.focus();
			return;
		}
		// verifica se usuario a ser editado possui perfil quando ativo
		if ( document.getElementById( 'usustatus_A' ).checked )
		{
			<? if ( !$perfisUsuario ) : ?>
				alert( 'Para atribuir status Ativo o usuário deve pelo menos um Perfil associado.' );
				return;
			<? endif; ?>
		}
		// FIM verifica se usuario a ser editado possui perfil quando ativo
		
       	if (!validaBranco(document.formulario.usufuncao, 'Função/Cargo')) return;
 	   	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
   	   	document.formulario.submit();

     }
 

</script>
