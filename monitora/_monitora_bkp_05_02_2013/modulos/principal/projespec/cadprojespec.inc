 <?
  /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:cadprojespec.inc
   finalidade: permitir a administração do projeto especial
   */
   $modulo=$_REQUEST['modulo'] ;//
   
function atualiza_ator($pje,$usu,$influ,$inter)
{
	// esta função verifica se o usuário informado já foi cadastrado no projeto. Se não foi faz inclusão. Se foi faz update.
	global $db;
	// verifica se o projeto temvisibilidade aberta a todos
	$sql = "Select usucpf from monitora.projeto_ator where usucpf='$usu' and pjeid=$pje";
	if ($db->pegaUm($sql))
	{
		$sql ="update monitora.projeto_ator set peainfluencia='$influ',peainteresse='$inter',peastatus='A' where usucpf='$usu' and pjeid=$pje";
	}
	else 
	{
		$sql ="insert into monitora.projeto_ator (pjeid,usucpf,peainfluencia,peainteresse) values ($pje,'".$usu."','$influ','$inter')";
	}
    $saida = $db->executar($sql);
}

function projetovisivel($cod)
{
	global $db;
	// verifica se o projeto temvisibilidade aberta a todos
	$sql="Select pjesnvisivel from monitora.projetoespecial where pjeid=$cod";
	if ($db->pegaUm($sql)=='t') return true;
	else return false;
}
  
if (! $db->testa_responsavel_projespec($_REQUEST['id']) and ! $_SESSION['pjeid'] and ! $db->testa_superuser() and ! $db->testa_digitador($_REQUEST['id'],'E') and ! $db->testa_altagestaopje($_REQUEST['id']) and ! projetovisivel($_REQUEST['id']))  
{
	?>
	<script>alert('Você não tem acesso a esta aplicação!!');
	history.back();
	</script>
	<?
	$db->close();
	exit();
}  
 
   
$pjeid = $_REQUEST['id'];
if (! is_numeric($pjeid)) $pjeid=$_REQUEST['id'];
//print 'pjeid='.$pjeid;exit();

if ($_REQUEST['id'])  $_SESSION['pjeid']= $pjeid;


function atualiza_acao($pjeid, $acacods )
	{
		global $db;

		// esta função tem por finalidade atualizar as ações do ppa associadas a um projeto.
		// como regra básica somente o gestor do projeto pode associar uma ação do ppa.
		// esta associação irá gerar um registro no plano de trabalho que só será válido quando o coordenador da ação aprovar a associação
		// após esta aprovação o sistema não deverá permitir retirar a associação
		// no caso de retirar a associação um email deverá ser enviado ao coordenador
		// um email deverá ser enviado no caso de associação
		$pjeid = (integer) $pjeid;
		if ( (!$pjeid) or (! is_array($acacods)))
		{
			return;
		}
		$sql = "select acaid from monitora.pjecomponente where pjeid=$pjeid and acaid is not null";
		$associacoes = $db->carregar($sql);
		if (is_array($associacoes) )
		{
		foreach ($associacoes as $associacao)
		{
			// procura no plano de trabalho se a ação ja foi associada e se já foi aprovada a associação
		    $sql = "select ptoid,acaid from monitora.planotrabalho where pjeid=$pjeid and acaid =".$associacao['acaid']. " and ptosntodaacao='t' and ptosnaprovado='f'";
		    $pode_excluir = $db->pegaLinha($sql);
			if ($pode_excluir)
			{
				$sql_remocao=" delete from monitora.pjecomponente where pjeid=$pjeid and acaid=".$pode_excluir['acaid'];
				$db->executar($sql_remocao);
				// agora apaga no plano de trabalho em cascata
				$sql_remocao_cascata = " delete from monitora.planotrabalho where ptoid in (select ptoid from  SELECT_ALL_CHILDS_FROM_PTO( ".$pode_excluir['ptoid']." )) or ptoid=".$pode_excluir['ptoid'];
				$db->executar($sql_remocao_cascata);
				//dbg($sql_remocao_cascata,1);
				//envia email ao coordenador avisando da exclusão da associação
				// descobre quem é o coordenador
   				$sql = "select a.prgcod,a.acacod as acacod2,a.unicod,a.loccod,a.acadsc from monitora.acao a where acaid = ".$pode_excluir['acaid'];
   				$res=$db->pegalinha($sql);
   				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

   				$sql = "select u.usunome as nomecriador,u.ususexo as sexocriador,u.usuemail as emailcriador,u.usufoneddd ||'-'||u.usufonenum as fonecriador from seguranca.usuario u where u.usucpf='".$_SESSION['usucpf']."'";
   				$res=$db->pegalinha($sql);
   				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;   
   
   				$sql = "select u.usunome,u.ususexo,u.usuemail from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on u.usucpf=ur.usucpf and ur.acaid=".$pode_excluir['acaid']." and ur.pflcod=1 and ur.rpustatus='A' ";
   				$res=$db->pegalinha($sql);
   				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
   				$assunto = 'Exclusão de Subação para apoiar Projeto Especial.';
   				$sexo = 'Prezado Sr.  ';
   				if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
   				$mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que foi excluída uma Subação na ação $prgcod - $acacod2.$unicod.$loccod - $acadsc, que está sob sua responsabilidade como Coordenador(a). V.Sa. não precisará se preocupar com a aprovação desta atividade em seu Plano de Trabalho.<br> Esta Subação foi excluída por:$nomecriador <br>E-Mail:$emailcriador<br>Telefone:$fonecriador";
   				$emailpara = $usuemail.','.$emailcriador;
   				$emailpara = $_SESSION['email_sistema'];
   				email($usunome, $emailpara, $assunto, $mensagem);// o email vai com cópia para quem criou
			}
		}
	}

		foreach ( (array) $acacods as $acacod )
		{
			if ($acacod) {
				
				// verifica primeiro se já não existe o registro
				$sql = "select acaid from monitora.pjecomponente where pjeid=$pjeid and acaid=$acacod";
				$existe=$db->pegaUm($sql);
				if (! $existe) {
					// então cria
				   $sql_insercao = "insert into monitora.pjecomponente (acaid, pjeid ) values ( " . $acacod . "," . $pjeid . ")";	               
				   $db->executar( $sql_insercao );
				   // após a inclusão precisa incluir um registro no plano de trabalho.
				   // pega alguns dados do projeto
				   $sql = "select to_char(pjedataini,'YYYY/mm/dd') as dataini, to_char(pjedatafim,'YYYY/mm/dd') as datafim from monitora.projetoespecial where pjeid=$pjeid";
				   $datas= $db->pegaLinha($sql);
				   $dtinipje=$datas['dataini'];
				   $dtfimpje=$datas['datafim'];
				   
				   // pega os dados da ação para utilizar no plano de trabalho				   
				   $sql = "select acacod as acacod2 ,acadsc,acadescricao,acafinalidade,acadetalhamento,taccod,acamesinicio,acamestermino,acaanotermino,acaanoinicio, unmcod,procod from monitora.acao where acaid=$acacod";
				   $res= $db->pegaLinha($sql);
				   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				   $_REQUEST['ptosnsoma']='f';
     			   $_REQUEST['ptosnaprovado']='f';
     			   if ($acadescricao) $_REQUEST['ptodescricao'] = $acadescricao; else $_REQUEST['ptodescricao'] = null;
     			   if ($acafinalidade) $_REQUEST['ptofinalidade'] = $acafinalidade; else $_REQUEST['ptofinalidade'] = null;
     			   if ($acadetalhamento) $_REQUEST['ptodetalhamento'] = $acadetalhamento; else $_REQUEST['ptodetalhamento'] = null;     			   
     			   if ($unmcod) $_REQUEST['unmcod'] = $unmcod; else $_REQUEST['unmcod'] = 2;  
     			   if ($procod) $_REQUEST['procod'] = $procod; else $_REQUEST['procod'] = null;  
     			   $dtinicio= $dtinipje;
     			   $dttermino= $dtfimpje;
     			   if ($taccod==1)
     			   {
     			   	// é uma ação do tipo projeto
     			   	if ($acamesinicio and $acaanoinicio) $dtinicio=$acaanoinicio.'/'.$acamesinicio.'/01';
     			   	else $dtinicio= $dtinipje;
     			   	if ($acamestermino and $acaanotermino) $dtinicio=$acaanotermino.'/'.$acamestermino.'/28';
     			   	else $dttermino= $dtfimpje;     			   	
     			   }
     			   // aqui se compara as datas
     			   if ($dtinipje > $dtinicio) $dtinicio=$dtinipje;
     			   if ($dtfimpje < $dttermino) $dttermino=$dtfimpje;     			   

    				 $_REQUEST['ptoid_pai']='null';
          			 $_REQUEST['ptosnsubacao']='t';  
          			 $_REQUEST['ptosntodaacao']='t';  
          			 // busca o valor de orçamento da ação
          			 
          			 $sql = "select sum( coalesce( rofautorizado, 0 ) ) as dotacao from financeiro.reporcfin where prgcod = '$prgcod' and acacod = '$acacod2' and unicod = '$unicod' and loccod = '$loccod' and rofano = '".$_SESSION['exercicio']."' "; 
                     $valor = $db->pegaUm($sql);
                     if (! $valor) $valor = 0;
          			 // busca o valor da meta
          			 $sql = "select sum(fisqtdeprevistoano) as meta from monitora.dadofisico where acaid=$acacod";
                     $meta = $db->pegaUm($sql);
                     if (! $meta) $meta = 0;                     
				     // achar o mais alto id para o código
					$sql = "select ptoordemprov from monitora.planotrabalho where pjeid=$pjeid and ptonivel = 1 and ptostatus='A' order by ptoordemprov desc limit 1";
					$maiorcod=$db->pegaum($sql);
					$novocod = $maiorcod+ 1;
					
				     // achar o mais alto id para o código só em ação
					$sql = "select ptoordemprovacao from monitora.planotrabalho where acaid=$acacod and ptonivel = 1  and ptostatus='A'  order by ptoordemprovacao desc limit 1";
					$maiorcodacao=$db->pegaum($sql);
					$novocodacao = $maiorcodacao+ 1;					

     				// achar a ordem na ação
					$sql = " select ptoordemacao from monitora.planotrabalho where acaid=$acacod order by ptoordemacao desc limit 1";
					$_REQUEST['ptoordemacao']=$db->pegaum($sql);
					if (! $_REQUEST['ptoordemacao'])$_REQUEST['ptoordemacao']=1;
     				// achar a ordem no projeto
					$sql = " select ptoordem from monitora.planotrabalho where pjeid=$pjeid order by ptoordem desc limit 1";
					$_REQUEST['ptoordem']=$db->pegaum($sql);					
		
					
    				$sql = "insert into monitora.planotrabalho (
  						  acaid,
						  ptodsc ,
						  ptoprevistoexercicio ,
						  unmcod ,
						  ptosnaprovado,
						  ptodata_ini,
						  ptodata_fim ,
						  ptoordem ,
						  ptodetalhamento,
						  ptodescricao,
						  ptofinalidade ,
						  ptocod ,
						  pjeid ,
						  ptosnsubacao ,
						  ptoordemacao,
						  ptoorigemppa ,
						  ptonivel,
						  ptoordemprov ,
						  procod,
						  ptovalorprevisto, 
						  ptosntodaacao ,
						  ptoordemprovacao ) 
						values (".
    				    $acacod.",'".
						$acadsc."',".
						$meta.",".    
						$unmcod.",'".
						'f'."','".
						$dtinicio."','".
						$dttermino."',".
						$_REQUEST['ptoordem'].",'".
						$_REQUEST['ptodetalhamento']."','".
						$_REQUEST['ptodescricao']."','".
						$_REQUEST['ptofinalidade']."','".
						$novocod."',".
						$pjeid.",'".
						't'."',".
						$_REQUEST['ptoordemacao'].",'".
						't'."',".
						'1'.",".
						$novocod.",".
						$_REQUEST['procod'].",".
						$valor.",'".
						"t"."',".
						$novocodacao.")";
						
						//dbg($sql,1);
   						$saida = $db->executar($sql); 
   						 
   						// envia e-mail ao coordenador avisando da criação da subação com cópia para o gerente do projeto especial
					   // descobre quem é o coordenador
					   $sql = "select a.prgcod,a.acacod,a.unicod,a.loccod,a.acadsc from monitora.acao a where acaid = ".$acacod;
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
					
					   $sql = "select u.usunome as nomecriador,u.ususexo as sexocriador,u.usuemail as emailcriador,u.usufoneddd ||'-'||u.usufonenum as fonecriador from seguranca.usuario u where u.usucpf='".$_SESSION['usucpf']."'";
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;   
					   
					   $sql = "select u.usunome,u.ususexo,u.usuemail from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on u.usucpf=ur.usucpf and ur.acaid=".$acacod." and ur.pflcod=1 and ur.rpustatus='A' ";
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
					   $assunto = 'Inclusão de Subação para apoiar Projeto Especial.';
					   $sexo = 'Prezado Sr.  ';
					   if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
					   $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que foi criada uma Subação na ação $prgcod - $acacod2.$unicod.$loccod - $acadsc, que está sob sua responsabilidade como Coordenador(a). Para que esta Subação possa ser utilizada no Projeto Especial, V.Sa. deverá, no cadastro do Plano de Trabalho da ação, aprovar a sua criação.<br> Esta Subação foi criada por:$nomecriador <br>E-Mail:$emailcriador<br>Telefone:$fonecriador";
					   $emailpara = $usuemail.','.$emailcriador;
					   $emailpara = $_SESSION['email_sistema'];
					   email($usunome, $emailpara, $assunto, $mensagem);// o email vai com cópia para quem criou
   						
				   
				}
			}

		}

	}   

function atualiza_pto( $pjeid, $ptocods )
	{
		global $db;
		// esta função tem por finalidade atualizar as subações do ppa associadas a um projeto.
		// como regra básica somente o gestor do projeto pode associar uma subação do ppa.
		// esta associação irá gerar um registro no plano de trabalho que só será válido quando o coordenador da ação aprovar a associação
		// após esta aprovação o sistema não deverá permitir retirar a associação
		// no caso de retirar a associação um email deverá ser enviado ao coordenador
		// um email deverá ser enviado no caso de associação
		
		$pjeid = (integer) $pjeid;
		if ( (!$pjeid) or (! is_array($ptocods)))
		{
			return;
		}
		$sql = "select ptoid,acaid from monitora.pjecomponente where pjeid=$pjeid and ptoid is not null";
		$associacoes = $db->carregar($sql);

		if (is_array($associacoes) )
		{
		foreach ($associacoes as $associacao)
		{

			// procura no plano de trabalho se a subação ja foi associada e se já foi aprovada a associação
		    $sql = "select ptoid,acaid from monitora.planotrabalho where pjeid=$pjeid and ptoid =".$associacao['ptoid']. " and ptosntodaacao='f' and ptosnaprovado='f'";
		    $pode_excluir = $db->pegaLinha($sql);
			if ($pode_excluir)
			{
				$sql_remocao=" delete from monitora.pjecomponente where pjeid=$pjeid and ptoid=".$pode_excluir['ptoid'];
				$db->executar($sql_remocao);
				// agora apaga no plano de trabalho em cascata
				$sql_remocao_cascata = " delete from monitora.planotrabalho where ptoid in (select ptoid from  SELECT_ALL_CHILDS_FROM_PTO( ".$pode_excluir['ptoid']." )) or ptoid=".$pode_excluir['ptoid'];
				$db->executar($sql_remocao_cascata);
	
				//envia email ao coordenador avisando da exclusão da associação
				// descobre quem é o coordenador
   				$sql = "select a.prgcod,a.acacod as acacod2,a.unicod,a.loccod,a.acadsc from monitora.acao a where acaid = ".$pode_excluir['acaid'];
   				$res=$db->pegalinha($sql);
   				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

   				$sql = "select u.usunome as nomecriador,u.ususexo as sexocriador,u.usuemail as emailcriador,u.usufoneddd ||'-'||u.usufonenum as fonecriador from seguranca.usuario u where u.usucpf='".$_SESSION['usucpf']."'";
   				$res=$db->pegalinha($sql);
   				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;   
   
   				$sql = "select u.usunome,u.ususexo,u.usuemail from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on u.usucpf=ur.usucpf and ur.acaid=".$pode_excluir['acaid']." and ur.pflcod=1 and ur.rpustatus='A' ";
   				$res=$db->pegalinha($sql);
   				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
   				$assunto = 'Exclusão de Subação para apoiar Projeto Especial.';
   				$sexo = 'Prezado Sr.  ';
   				if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
   				$mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que foi excluída uma Subação na ação $prgcod - $acacod2.$unicod.$loccod - $acadsc, que está sob sua responsabilidade como Coordenador(a). V.Sa. não precisará se preocupar com a aprovação desta atividade em seu Plano de Trabalho.<br> Esta Subação foi excluída por:$nomecriador <br>E-Mail:$emailcriador<br>Telefone:$fonecriador";
   				$emailpara = $usuemail.','.$emailcriador;
   				$emailpara = $_SESSION['email_sistema'];
   				email($usunome, $emailpara, $assunto, $mensagem);// o email vai com cópia para quem criou
			}
		}
	}
/*
		foreach ( (array) $ptocods as $ptocod )
		{
			if ($ptocod) {
				
				// verifica primeiro se já não existe o registro
				$sql = "select ptoid from monitora.pjecomponente where pjeid=$pjeid and ptoid=$ptocod";
				$existe=$db->pegaUm($sql);
				if (! $existe) {
					// então cria
				   $sql_insercao = "insert into monitora.pjecomponente (acaid, pjeid,ptoid ) values ( " . $_REQUEST['acaid'] . "," . $pjeid . ",$ptocod)";	               
				   $db->executar( $sql_insercao );
				   // após a inclusão precisa incluir um registro no plano de trabalho.
				   // pega alguns dados do projeto
				   $sql = "select to_char(pjedataini,'YYYY/mm/dd') as dataini, to_char(pjedatafim,'YYYY/mm/dd') as datafim from monitora.projetoespecial where pjeid=$pjeid";
				   $datas= $db->pegaLinha($sql);
				   $dtinipje=$datas['dataini'];
				   $dtfimpje=$datas['datafim'];
				   
				   // pega os dados da ação para utilizar no plano de trabalho				   
				   $sql = "select acacod as acacod2 ,acadsc,acadescricao,acafinalidade,acadetalhamento,taccod,acamesinicio,acamestermino,acaanotermino,acaanoinicio, unmcod,procod from monitora.acao where acaid=$acacod";
				   $res= $db->pegaLinha($sql);
				   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				   $_REQUEST['ptosnsoma']='f';
     			   $_REQUEST['ptosnaprovado']='f';
     			   if ($acadescricao) $_REQUEST['ptodescricao'] = $acadescricao; else $_REQUEST['ptodescricao'] = null;
     			   if ($acafinalidade) $_REQUEST['ptofinalidade'] = $acafinalidade; else $_REQUEST['ptofinalidade'] = null;
     			   if ($acadetalhamento) $_REQUEST['ptodetalhamento'] = $acadetalhamento; else $_REQUEST['ptodetalhamento'] = null;     			   
     			   if ($unmcod) $_REQUEST['unmcod'] = $unmcod; else $_REQUEST['unmcod'] = 2;  
     			   if ($procod) $_REQUEST['procod'] = $procod; else $_REQUEST['procod'] = null;  
     			   $dtinicio= $dtinipje;
     			   $dttermino= $dtfimpje;
     			   if ($taccod==1)
     			   {
     			   	// é uma ação do tipo projeto
     			   	if ($acamesinicio and $acaanoinicio) $dtinicio=$acaanoinicio.'/'.$acamesinicio.'/01';
     			   	else $dtinicio= $dtinipje;
     			   	if ($acamestermino and $acaanotermino) $dtinicio=$acaanotermino.'/'.$acamestermino.'/28';
     			   	else $dttermino= $dtfimpje;     			   	
     			   }
     			   // aqui se compara as datas
     			   if ($dtinipje > $dtinicio) $dtinicio=$dtinipje;
     			   if ($dtfimpje < $dttermino) $dttermino=$dtfimpje;     			   

    				 $_REQUEST['ptoid_pai']='null';
          			 $_REQUEST['ptosnsubacao']='t';  
          			 $_REQUEST['ptosntodaacao']='t';  
          			 // busca o valor de orçamento da ação
          			 
          			 $sql = "select sum( coalesce( rofautorizado, 0 ) ) as dotacao from financeiro.reporcfin where prgcod = '$prgcod' and acacod = '$acacod2' and unicod = '$unicod' and loccod = '$loccod' and rofano = '".$_SESSION['exercicio']."' "; 
                     $valor = $db->pegaUm($sql);
                     if (! $valor) $valor = 0;
          			 // busca o valor da meta
          			 $sql = "select sum(fisqtdeprevistoano) as meta from monitora.dadofisico where acaid=$acacod";
                     $meta = $db->pegaUm($sql);
                     if (! $meta) $meta = 0;                     
				     // achar o mais alto id para o código
					$sql = "select ptoordemprov from monitora.planotrabalho where pjeid=$pjeid and ptonivel = 1 and ptostatus='A' order by ptoordemprov desc limit 1";
					$maiorcod=$db->pegaum($sql);
					$novocod = $maiorcod+ 1;
					
				     // achar o mais alto id para o código só em ação
					$sql = "select ptoordemprovacao from monitora.planotrabalho where acaid=$acacod and ptonivel = 1  and ptostatus='A'  order by ptoordemprovacao desc limit 1";
					$maiorcodacao=$db->pegaum($sql);
					$novocodacao = $maiorcodacao+ 1;					

     				// achar a ordem na ação
					$sql = " select ptoordemacao from monitora.planotrabalho where acaid=$acacod order by ptoordemacao desc limit 1";
					$_REQUEST['ptoordemacao']=$db->pegaum($sql);
					if (! $_REQUEST['ptoordemacao'])$_REQUEST['ptoordemacao']=1;
     				// achar a ordem no projeto
					$sql = " select ptoordem from monitora.planotrabalho where pjeid=$pjeid order by ptoordem desc limit 1";
					$_REQUEST['ptoordem']=$db->pegaum($sql);					
		
					
    				$sql = "insert into monitora.planotrabalho (
  						  acaid,
						  ptodsc ,
						  ptoprevistoexercicio ,
						  unmcod ,
						  ptosnaprovado,
						  ptodata_ini,
						  ptodata_fim ,
						  ptoordem ,
						  ptodetalhamento,
						  ptodescricao,
						  ptofinalidade ,
						  ptocod ,
						  pjeid ,
						  ptosnsubacao ,
						  ptoordemacao,
						  ptoorigemppa ,
						  ptonivel,
						  ptoordemprov ,
						  procod,
						  ptovalorprevisto, 
						  ptosntodaacao ,
						  ptoordemprovacao ) 
						values (".
    				    $acacod.",'".
						$acadsc."',".
						$meta.",".    
						$unmcod.",'".
						'f'."','".
						$dtinicio."','".
						$dttermino."',".
						$_REQUEST['ptoordem'].",'".
						$_REQUEST['ptodetalhamento']."','".
						$_REQUEST['ptodescricao']."','".
						$_REQUEST['ptofinalidade']."','".
						$novocod."',".
						$pjeid.",'".
						't'."',".
						$_REQUEST['ptoordemacao'].",'".
						't'."',".
						'1'.",".
						$novocod.",".
						$_REQUEST['procod'].",".
						$valor.",'".
						"t"."',".
						$novocodacao.")";
						
						//dbg($sql,1);
   						$saida = $db->executar($sql); 
   						 
   						// envia e-mail ao coordenador avisando da criação da subação com cópia para o gerente do projeto especial
					   // descobre quem é o coordenador
					   $sql = "select a.prgcod,a.acacod,a.unicod,a.loccod,a.acadsc from monitora.acao a where acaid = ".$acacod;
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
					
					   $sql = "select u.usunome as nomecriador,u.ususexo as sexocriador,u.usuemail as emailcriador,u.usufoneddd ||'-'||u.usufonenum as fonecriador from seguranca.usuario u where u.usucpf='".$_SESSION['usucpf']."'";
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;   
					   
					   $sql = "select u.usunome,u.ususexo,u.usuemail from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on u.usucpf=ur.usucpf and ur.acaid=".$acacod." and ur.pflcod=1 and ur.rpustatus='A' ";
					   $res=$db->pegalinha($sql);
					   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
					   $assunto = 'Inclusão de Subação para apoiar Projeto Especial.';
					   $sexo = 'Prezado Sr.  ';
					   if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
					   $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que foi criada uma Subação na ação $prgcod - $acacod2.$unicod.$loccod - $acadsc, que está sob sua responsabilidade como Coordenador(a). Para que esta Subação possa ser utilizada no Projeto Especial, V.Sa. deverá, no cadastro do Plano de Trabalho da ação, aprovar a sua criação.<br> Esta Subação foi criada por:$nomecriador <br>E-Mail:$emailcriador<br>Telefone:$fonecriador";
					   $emailpara = $usuemail.','.$emailcriador;
					   $emailpara = $_SESSION['email_sistema'];
					   email($usunome, $emailpara, $assunto, $mensagem);// o email vai com cópia para quem criou
   						
				   
				}
			}

		}
		*/

	}	
	
function atualiza_altagestao( $pjeid, $usucpfs )
	{
		global $db;
		$pjeid = (integer) $pjeid;
		if ( !$pjeid or ! is_array($usucpfs))
		{
			return;
		}
		$sql_remocao = "delete from monitora.usuarioresponsabilidade where pflcod=58 and pjeid=".$pjeid;
		$db->executar( $sql_remocao );
		 
		foreach ( (array) $usucpfs as $usucpf )
		{
			if ($usucpf)
			{
			$sql_insercao = "insert into monitora.usuarioresponsabilidade (usucpf, pjeid,pflcod ) values ( '" . $usucpf . "'," . $pjeid . ",58)";		
			$db->executar( $sql_insercao );
    	    atualiza_ator($pjeid,$usucpf,'A','D');  			
			}
		}
	}
		
if ($_REQUEST['act']== 'inserir')
{
  // inclusão de projeto especial
    $sql= "select pjecod from monitora.projetoespecial where (pjestatus='A') and (pjecod='".str_to_upper($_REQUEST['pjecod'])."' and (ungcod='".$_REQUEST['ungcod']."') or pjedsc='".str_to_upper($_REQUEST['pjedsc'])."')";
    $usu = $db->recuperar($sql);
	if (is_array($usu)) {
	   // existe projeto identico, logo, tem que bloquear
	   $sql="select ungabrev from unidadegestora where ungcod='".$_REQUEST['ungcod']."'";
	   $ungcod=$db->pegaUm($sql);
	   ?>
	      <script>
	         alert ('O Projeto: <?=$ungabrev.'-'.$_REQUEST['pjecod']?> já se encontra cadastrado com o mesmo código ou o mesmo nome no sistema. Os nomes dos projetos não podem ser repetidos.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
	   if (! $_REQUEST['pjeprevistoano']) $_REQUEST['pjeprevistoano']=0;
	   if (! $_REQUEST['pjevlrano']) $_REQUEST['pjevlrano']=0;	   
	   if (! $_REQUEST['procod']) $_REQUEST['procod']=0;	   
	   if (! $_REQUEST['unmcod']) $_REQUEST['unmcod']=0;
	   if (! $_REQUEST['usucpfcoord'] ) $_REQUEST['usucpfcoord']=null;
	   if (! $_REQUEST['pjesndatafechada'] ) $_REQUEST['pjesndatafechada']='f';
	   if (! $_REQUEST['pjesnvisivel'] ) $_REQUEST['pjesnvisivel']='f';	   
	   		   
        $sql = "insert into monitora.projetoespecial (pjestatus,pjecod,pjedsc,prsano,procod,unmcod,pjeprevistoano,pjevlrano,pjedescricao,pjefinalidade,ungcod,pjedataini,pjedatafim,pjesndatafechada,pjesnvisivel, tpscod,pjeexcecao) values ('A',".
   "'".str_to_upper($_REQUEST['pjecod'])."',".
   "'".str_to_upper($_REQUEST['pjedsc'])."','".$_SESSION['exercicio']."',".$_REQUEST['procod'].",".$_REQUEST['unmcod'].",".$_REQUEST['pjeprevistoano'].",".$_REQUEST['pjevlrano'].",'".$_REQUEST['pjedescricao']."','".$_REQUEST['pjefinalidade']."','".$_REQUEST['ungcod']."','".$_REQUEST['pjedataini']."','".$_REQUEST['pjedatafim']."','".$_REQUEST['pjesndatafechada']."','".$_REQUEST['pjesnvisivel']."',10,'".$_REQUEST['pjeexcecao']."')";
       $saida = $db->executar($sql);
       	$sql =  "Select pjeid from monitora.projetoespecial where oid = ".pg_last_oid($saida);
	    $pjeid = $db->pegaUm($sql);
       
       atualiza_pto($pjeid, $_REQUEST['ptocod'] );
       atualiza_acao($pjeid, $_REQUEST['acacod'] ); 
       atualiza_altagestao($pjeid, $_REQUEST['usucpf'] );       
       // cria registro em usuárioresponsabilidade
       $sql ="insert into monitora.usuarioresponsabilidade (pjeid,pflcod,usucpf,prsano) values ($pjeid,12,'".$_SESSION['usucpf']."','".$_SESSION['exercicio']."')";
       $saida = $db->executar($sql);  
       // cria registro em projeto ator  
       atualiza_ator($pjeid,$_SESSION['usucpf'],'T','D');
             
       $db->commit();

    if ($_REQUEST['usucpfcoord'] and $_REQUEST['usucpfcoord'] <>null) {    	
    	// inclui o usuário na tabela de usuário responsabilidade
    		$sql = "insert into monitora.usuarioresponsabilidade (pjeid,usucpf,pflcod,prsano) values ($pjeid,'".$_REQUEST['usucpfcoord']."',47,'".$_SESSION['exercicio']."')";
    		$db->executar($sql);
    		// verifica se o usuario possui o perfil 47 na tabela perfilusuario
    		$sql ="select usucpf from seguranca.perfilusuario where pflcod=47 and usucpf='".$_REQUEST['usucpfcoord']."'";
    		if (! $db->pegaUm($sql))
    		{
    			// insere
    			$sql ="insert into seguranca.perfilusuario (pflcod,usucpf) values (47,'".$_REQUEST['usucpfcoord']."')";
    			$db->executar($sql);
    		}
    	// cria registro em projeto_ator
    	    atualiza_ator($pjeid,$_REQUEST['usucpfcoord'],'T','D');  
     	 	$db->commit();
    	 	
    // envia email para o Gerente avisando o ocorrido
		$sql="select ug.ungabrev,p.pjecod ,p.pjedsc from monitora.projetoespecial p inner join unidadegestora ug on ug.ungcod=p.ungcod where p.pjeid=".$pjeid;
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;       
         // envia email
        $assunto = 'Inclusão como responsável em Projeto Especial';
		$sexo = 'Prezado Sr.  ';
		$sql="select ususexo,usunome,usuemail from seguranca.usuario where usucpf='".$_REQUEST['usucpfcoord']."'";
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
		
        $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que seu nome foi associado, no SIMEC, como responsável do Projeto Especial ".$ungabrev.$pjecod.' - '.$pjedsc;
        email($usunome, $usuemail, $assunto, $mensagem);
    }    
    	    $db->sucesso($modulo);   
}

if ($_REQUEST['act']== 'alterar')
{
  // alteração de projeto especial
	   if (! $_REQUEST['pjeprevistoano']) $_REQUEST['pjeprevistoano']=0;
	   if (! $_REQUEST['pjevlrano']) $_REQUEST['pjevlrano']=0;	
	   if (! $_REQUEST['procod']) $_REQUEST['procod']=0;	   
	   if (! $_REQUEST['unmcod']) $_REQUEST['unmcod']=0;
	   $pjeid=$_SESSION['pjeid'];
	   if (! $_REQUEST['usucpfcoord'] ) $_REQUEST['usucpfcoord']=null;
	   if (! $_REQUEST['pjesndatafechada'] ) $_REQUEST['pjesndatafechada']='f';	
	   if (! $_REQUEST['pjesnvisivel'] ) $_REQUEST['pjesnvisivel']='f';	 

	   // verifica se pode alterar as datas
	   $sql = "select ptodata_ini from monitora.planotrabalho where ptodata_ini < '".$_REQUEST['pjedataini']."' and pjeid=".$_SESSION['pjeid'] ." and ptostatus='A' order by ptodata_ini limit 1";
	   $datamenor=$db->pegaUm($sql);
	   if ($datamenor) $_REQUEST['pjedataini']=$datamenor;
	   
	   $sql = "select ptodata_fim from monitora.planotrabalho where ptodata_fim > '".$_REQUEST['pjedatafim']."' and pjeid=".$_SESSION['pjeid']." and ptostatus='A' order by ptodata_fim desc limit 1";
	   $datamaior=$db->pegaUm($sql);
	   if ($datamaior) $_REQUEST['pjedatafim']=$datamaior;	   
	   
	   
	$sql="select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and pjeid=".$_SESSION['pjeid']." and pflcod=47 limit 1";
	$cpforiginal=$db->pegaum($sql);
    $sql = "update monitora.projetoespecial set pjedsc ='".str_to_upper($_REQUEST['pjedsc'])."', procod=".$_REQUEST['procod'].",unmcod=".$_REQUEST['unmcod'].",pjeprevistoano=".$_REQUEST['pjeprevistoano']." , pjevlrano=".$_REQUEST['pjevlrano'].",  pjedataini='".$_REQUEST['pjedataini']."', pjedatafim='".$_REQUEST['pjedatafim']."' ,pjefinalidade='".$_REQUEST['pjefinalidade']."', pjedescricao='".$_REQUEST['pjedescricao']."',pjesndatafechada='".$_REQUEST['pjesndatafechada']."', pjesnvisivel='".$_REQUEST['pjesnvisivel']."',tpscod=".$_REQUEST['tpscod'].",pjeexcecao='".$_REQUEST['pjeexcecao']."' where pjeid=".$_SESSION['pjeid'];

       $saida = $db->executar($sql);
       atualiza_pto($pjeid, $_REQUEST['ptocod'] );
       atualiza_acao($pjeid, $_REQUEST['acacod'] ); 
       atualiza_altagestao($pjeid, $_REQUEST['usucpf'] ); 
	   $db->commit();
	   
	   // envia email para o responsável avisando do ocorrido se e somente se for diferente do original. Neste caso envia email para os dois.
	    if ($cpforiginal <> $_REQUEST['usucpfcoord'])
	    {      if ($_REQUEST['usucpfcoord'] and $_REQUEST['usucpfcoord'] <>null) {
	    	// então mudou de responsável
	    	// desabilita o responsavel anterior

	    	$sql = "update monitora.usuarioresponsabilidade set rpustatus='I' where pjeid=".$_SESSION['pjeid']." and usucpf='$cpforiginal' and pflcod=47";
    		$db->executar($sql);
	    	// inclui o novo Gerente na tabela de usuário responsabilidade
    		$sql = "insert into monitora.usuarioresponsabilidade (pjeid,usucpf,pflcod,prsano) values ($pjeid,'".$_REQUEST['usucpfcoord']."',47,'".$_SESSION['exercicio']."')";
    		$db->executar($sql);
    		// verifica se o usuario possui o perfil 47 na tabela perfilusuario
    		$sql ="select usucpf from seguranca.perfilusuario where pflcod=47 and usucpf='".$_REQUEST['usucpfcoord']."'";
    		if (! $db->pegaUm($sql))
    		{
    			// insere
    			$sql ="insert into seguranca.perfilusuario (pflcod,usucpf) values (47,'".$_REQUEST['usucpfcoord']."')";
    			$db->executar($sql);
    		}
    	 	 // cria registro em projeto ator 
    	    atualiza_ator($pjeid,$_REQUEST['usucpfcoord'],'T','D');  

    	 	$db->commit();
	    	// primeiro envia email para o novo responsável
            $sql="select ug.ungabrev,p.pjecod ,p.pjedsc from monitora.projetoespecial p inner join unidadegestora ug on ug.ungcod=p.ungcod where p.pjeid=".$pjeid;
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;       
         // envia email
        $assunto = 'Inclusão como responsável em Projeto Especial';
		$sexo = 'Prezado Sr.  ';
		$sql="select ususexo,usunome,usuemail from seguranca.usuario where usucpf='".$_REQUEST['usucpfcoord']."'";
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
		
        $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que seu nome foi associado, no SIMEC, como responsável do Projeto Especial ".$ungabrev.$pjecod.' - '.$pjedsc;
        email($usunome, $usuemail, $assunto, $mensagem);
	    }
             if ($cpforiginal)
             {
             	 // inativa do cadastro o responsável
             	 $sql = "update monitora.usuarioresponsabilidade set rpustatus='I' where pjeid=".$_SESSION['pjeid']." and usucpf='$cpforiginal' and pflcod=47";
    		$db->executar($sql);
    		
    	 	$db->commit();
    	 	
             // agora envia email para o que deixou de ser responsavel
             $assunto = 'Exclusão como responsável em Projeto Especial';
		     $sexo = 'Prezado Sr.  ';
		     $sql="select ususexo,usunome,usuemail from seguranca.usuario where usucpf='".$cpforiginal."'";
		     $res=$db->pegalinha($sql);
		     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		     if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
		     $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que seu nome foi retirado, no SIMEC, como responsável do Projeto Especial ".$ungabrev.$pjecod.' - '.$pjedsc;
             email($usunome, $usuemail, $assunto, $mensagem);   
             }                       	
	    }
	    $db->sucesso($modulo);
	   
}




if ($_SESSION['pjeid']) 
{
	    $pjeid = $_SESSION['pjeid'];

        $sql= "select pe.*,ug.ungabrev from monitora.projetoespecial pe left join unidadegestora ug on ug.ungcod=pe.ungcod where pjeid=$pjeid";
		//print $sql;
        $saida = $db->recuperar($sql,$res);        
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
        
        $sql = "select usucpf from monitora.usuarioresponsabilidade where pjeid=$pjeid and pflcod=47 and rpustatus ='A' limit 1";

        $usucpfcoord=$db->pegaUm($sql);

 }
 else 
 {
	$pjecod = $_REQUEST['pjecod'];
	$pjedsc =$_REQUEST['pjedsc'];
	$pjeid=0;
 }

  $autoriza=false;
  if ($db->testa_responsavel_projespec()) {
  	$autoriza = true;
  }

  $digitador=0;
  if ($_SESSION['pjeid'])
  {
  if ($db->testa_digitador($_SESSION['pjeid'],'E')) {
  	$digitador = true;
  }
  }

  $coordpje=false;
  if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) {
  	$coordpje = true;
  }
   $superuser = false;
   // verific se é super-usuário
  if ($db->testa_superuser())  {
  	$coordpje = true; 
  	$superuser = true;

  }
  

 $alterar=false;
 if ($db->testa_superuser() ) $alterar=true;
  //Verifica se é o gestor do projeto 
  if ($_REQUEST['id'])
  $sql="select ur.rpuid from monitora.usuarioresponsabilidade ur where ur.pflcod=12 and ur.pjeid=".$_REQUEST['id']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' order by ur.rpudata_inc limit 1";
  else if ( $_SESSION['pjeid'])
  $sql="select ur.rpuid from monitora.usuarioresponsabilidade ur where ur.pflcod=12 and ur.pjeid=".$_SESSION['pjeid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' order by ur.rpudata_inc limit 1";
  else
  $sql="select ur.rpuid from monitora.usuarioresponsabilidade ur where ur.pflcod=12 and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' order by ur.rpudata_inc limit 1";


   //Caso seja gestor de projeto ou super usuário mostra botão inclui
    if ($db->pegaum($sql)) {$alterar=true;}
 
 
// select ur.rpuid from monitora.usuarioresponsabilidade ur where ur.pflcod=12 and ur.pjeid= and ur.usucpf='' and ur.rpustatus='A' order by ur.rpudata_inc limit 1"


 
 
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba($abacod_tela,$url,'');
if ($_REQUEST['acao']=='I') $titulo_modulo='Inclusão de Projeto Especial';
else $titulo_modulo='Alteração de Projeto Especial';
monta_titulo($titulo_modulo,'');


// variáveis gerais

$sql_acao = "SELECT DISTINCT 
				a.acaid AS codigo, 
				a.prgcod||'.'||a.acacod ||'.'||a.unicod ||'.'||
				a.loccod ||' - ' || a.acadsc ||
				' Coordenador --> '|| u.usunome || ' ('|| ee.entnome ||')' ||
				' Fone:'||u.usufoneddd||'-'||u.usufonenum || 
				'-- E-Mail --> ' || u.usuemail AS descricao 
			 FROM 
			 	monitora.acao a 
			 LEFT JOIN 
			 	monitora.usuarioresponsabilidade ur ON ur.acaid = a.acaid AND
			 										   ur.pflcod = 1 
			 INNER JOIN 
				seguranca.usuario u ON u.usucpf = ur.usucpf AND 
									   u.suscod = 'A' 
			 LEFT JOIN 
				entidade.entidade ee ON ee.entid = u.entid 
			 WHERE 
				a.prgano = '" . $_SESSION['exercicio'] . "' AND 
				acasnrap = 'f' AND 
				a.acaid NOT IN ( SELECT 
									acaid 
								 FROM 
								 	monitora.planotrabalho 
								 WHERE 
								 	ptosntodaacao = 't' AND 
								 	ptosnaprovado = 't') 
			 ORDER BY 
			 	descricao";

// Caso tenha pjeid, a query muda
$clausula_pjeid = ($_SESSION['pjeid']) ? " AND p.pjeid = " . $_SESSION['pjeid'] : ''; 
 
$sql_pto = "SELECT DISTINCT 
				p.ptoid AS codigo, 
				p.ptodsc ||
				'- Ação: '||a.acacod ||'-'||a.acadsc ||
				' Coordenador --> ' || u.usunome || ' ('|| ee.entnome ||')' ||
				' Fone:'||u.usufoneddd||'-'||u.usufonenum || 
				'-- E-Mail --> ' || u.usuemail AS descricao 
			FROM 
				monitora.planotrabalho p 
			INNER JOIN 
				monitora.acao a ON a.acaid = p.acaid 
			LEFT JOIN 
				monitora.usuarioresponsabilidade ur ON ur.acaid = a.acaid AND 
													   ur.pflcod = 1 
			LEFT JOIN 
				seguranca.usuario u ON u.usucpf = ur.usucpf
			LEFT JOIN 
				entidade.entidade ee ON ee.entid = u.entid 
			WHERE 
				p.ptosnsubacao = 't' AND 
				a.acaid NOT IN ( SELECT 
									acaid 
								 FROM 
								 	monitora.planotrabalho 
								 WHERE 
								 	ptosntodaacao = 't') " . $clausula_pjeid . "
			ORDER BY 
				descricao";

 
$sql_usuario_cad = "select distinct u.usucpf as codigo, u.usunome as descricao from seguranca.usuario u inner join seguranca.perfilusuario pu on pu.usucpf=u.usucpf and pu.pflcod=58 inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.sisid=".$_SESSION['sisid']." and us.susstatus='A' where u.usustatus='A' order by descricao";
//

?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name='act' >
<input type='hidden' name='ver' value=<?=$_REQUEST['ver']?>>
<input type='hidden' name='exclui' value=0>
<input type='hidden' name="pjeid" value=<?=$pjeid?>>

   <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <?
    if (! $pjeid)
    {
    	$habil='S' ;?>    
        <tr>
            <td align='right' class="SubTituloDireita">UG:</td>
	        <td><? $sql = "select ungcod as CODIGO,ungabrev ||' - '|| ungdsc as DESCRICAO from unidadegestora where ungstatus='A'  order by ungabrev ";
                  $db->monta_combo("ungcod",$sql,$habil,"Escolha a UG que irá gerenciar o Projeto",'','','Escolha a UG que irá gerenciar o Projeto. Todo Projeto Especial tem que ser gerido por uma UG.',400,'S');?>
                  
           </td>
       </tr>    
       <tr>
           <td align='right' class="SubTituloDireita">Código:</td>
	       <td>
	       <?=campo_texto('pjecod','S','S','',13,11,'','','','Entre com o código com até 11 caracteres!');?>
           </td>
       </tr>
     <?
    }
     else 
    { $habil='N' ;?> 
       <input type="hidden" name="ungcod" value="<?=$ungcod?>">
       <tr>
           <td align='right' class="SubTituloDireita">Código:</td>
	       <td>
	       <?=$ungabrev.'-'.campo_texto('pjecod','S','S','',13,11,'','');?>
           </td>
       </tr>
      <?}?>
      
      <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
	    <td><?=campo_texto('pjedsc','S','S','',77,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data Início:</td>
        <td>
		<?=campo_data('pjedataini', 'S','S','','S');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data Término:</td>
        <td>
	        <?=campo_data('pjedatafim', 'S','S','','S');?>
	</td>
      </tr>   
       <tr>
        <td align='right' class="SubTituloDireita">Congela as datas?</td>
        <td>
            <input type="radio" name="pjesndatafechada" value="t" <?=($pjesndatafechada=='t'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="pjesndatafechada" value="f" <?=($pjesndatafechada=='f'?"CHECKED":"")?>> Não
         </td>
       </tr>         
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
        <td><?=campo_textarea('pjedescricao','S','S','',80,3,'','','',' Campo utilizado para explicar o motivo da origem do Projeto Especial, como, por exemplo, sua composição, raízes históricas, etc.');?></td>
      </tr>  
      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
        <td><?=campo_textarea('pjefinalidade','S','S','',80,3,'','','','Campo utilizado para explicar para que se destina este Projeto Especial, ou seja, o que se pretende alcançar com sua criação.');?></td>         
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Exceção:</td>
        <td><?=campo_textarea('pjeexcecao','S','S','',80,3,'','','','Descreva aqui o que o Projeto NÃO irá fazer ou abordar, procurando definir, assim, as suas fronteiras.');?></td>         
      </tr>      
      <tr >
        <td align='right' class="SubTituloDireita">Produto:</td>
        <td >
	<?
	  $sql = "select procod as CODIGO,prodsc as DESCRICAO from produto where prostatus='A'  order by prodsc ";
	  $db->monta_combo("procod",$sql,'S',"Escolha o Produto do Projeto",'','','Entre com o produto do projeto. Este campo NÃO é obrigatório',400);
	?>
	</td>
      </tr>   
             <tr >
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td >
	<?
	  $sql = "select unmcod as CODIGO,unmdsc as DESCRICAO from unidademedida where unmstatus='A'  order by unmdsc ";
	  $db->monta_combo("unmcod",$sql,'S',"Selecione a Unidade de Medida",'','','Escolha a unidade de medida do projeto. Este campo NÃO é obrigatório.',400);
	?>
	</td>
      </tr>  
      </tr>   
      <tr >
        <td align='right' class="SubTituloDireita">Meta Física:</td>
        <td >
	<?=campo_texto('pjeprevistoano','N','S','',10,10,'###########','','left','Meta que se pretende alcançar. Esta meta precisa ser acompanhada neste sistema!');?> (Valores em inteiros)
		</td>
      </tr>  
      <tr >
        <td align='right' class="SubTituloDireita">Orçamentário:</td>
        <td >
	<?=campo_texto('pjevlrano','N','S','',20,18,'#############.##','','rigth','Orçamento que se pretende alocar a este Projeto e que deverá ser acompanhado neste sistema. Tenha em mente que o SIAFI não terá condições de acompanhar os seus lançamentos.');?> (Valores em reais inteiros)
		</td>
      </tr> 
        <tr>
        <td align='right' class="SubTituloDireita">Permite que seja visível a outros usuários?</td>
        <td>
            <input type="radio" name="pjesnvisivel" value="t" <?=($pjesnvisivel=='t'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="pjesnvisivel" value="f" <?=($pjesnvisivel=='f'?"CHECKED":"")?>> Não
         </td>
       </tr> 
     <? if ($pjeid) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Ações componentes</td>
        <td>
        	<?
        	$sql_auxiliar_ac = "select distinct a.acaid as codigo, a.prgcod||'.'||a.acacod ||'.'||a.unicod ||'.'||a.loccod || ' - ' || a.acadsc as descricao from monitora.pjecomponente pjc   inner join monitora.acao a on pjc.acaid=a.acaid inner join monitora.planotrabalho p on p.acaid=a.acaid and p.pjeid = ".$_SESSION['pjeid']." and p.ptosntodaacao='t' where pjc.pjeid=$pjeid  order by descricao"; 
            $acacod = $db->carregar( $sql_auxiliar_ac ); 
       		combo_popup( 'acacod', $sql_acao, 'Selecione a(s) Ação(s)', '360x460',0,null ,null,'S',null,null,5)
       		?>        
             </td>      
      </tr>                  
      <tr>
        <td align='right' class="SubTituloDireita">Subações componentes</td>
        <td>
        	<?
        	$sql_auxiliar_sa = "select distinct p.ptoid as codigo, p.ptodsc as descricao from monitora.pjecomponente pjc inner join monitora.planotrabalho p on pjc.ptoid=p.ptoid and p.ptosnsubacao='t' and p.ptosntodaacao='f' where pjc.pjeid=$pjeid order by descricao"; 
             $ptocod = $db->carregar( $sql_auxiliar_sa );  
        	 //combo_popup( 'ptocod', $sql_pto, 'Selecione a(s) Subação(s)', '360x460',0)
        	 combo_popup( 'ptocod', $sql_pto, 'Selecione a(s) Subação(s)', '360x460',0,null ,null,'S',null,null,5)
        	 ?> 
        	 <? if ($autoriza) {?><img border="0" src="../imagens/gif_inclui.gif" title="Incluir Subação." onclick="inclui_subacao('<?=md5_encrypt($_SESSION['pjeid'],'')?>')"><?}?>        
         </td>      
      </tr> 
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Gerente:</td>
	    <td><?
	    	  $sql = "select distinct u.usucpf as CODIGO,u.usucpf ||'-'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as DESCRICAO, u.usunome from seguranca.usuario u 
where u.usucpf in (select pu.usucpf from seguranca.perfilusuario pu where pu.pflcod=47) 
or u.usucpf in (select us.usucpf from seguranca.usuario_sistema us where us.pflcod=47)
and u.usustatus='A' order by usunome ";
        	  $db->monta_combo("usucpfcoord",$sql,'S',"Selecione o Responsável",'','','Escolha quem será o Gerente do Projeto',400);
	    ?></td>
      </tr> 
           
     <tr>
        <td align='right' class="SubTituloDireita">Usuários Alta-Gestão:</td>
        <td>
        	<?
        	$sql_auxiliar_cad = "select distinct u.usucpf as codigo, u.usunome as descricao from monitora.usuarioresponsabilidade up inner join seguranca.usuario u on up.usucpf=u.usucpf where up.pflcod=58 and up.pjeid=$pjeid order by descricao"; 
            $usucpf = $db->carregar( $sql_auxiliar_cad ); 
       		combo_popup( 'usucpf', $sql_usuario_cad, 'Selecione o(s) Usuário(s)', '360x460',0,null,null,'S',null,null,5)
       		?>        
             </td>      
      </tr> 
                       	<tr>
		<td class="SubTituloDireita">Arquivos vinculados ao projeto:</td>
		<td>
			<? 
				$sql=sql_vincula_arquivo('monitora.projetoespecial',$_SESSION[ 'pjeid' ],'pjeid');
				$insere=0; 
				if ($coordpje or $digitador) $insere=1;		   
     			popup_arquivo( 'arquivo_pje', $sql, 'pjeid',$_SESSION[ 'pjeid' ], $insere, 400, 400,'monitora.projetoespecial' );	
				
			?>
		</td>
	</tr>
      <tr >
        <td align='right' class="SubTituloDireita">Situação do Projeto:</td>
        <td >
	<?
	  $sql = "select tpscod as CODIGO,tpsdsc as DESCRICAO from tiposituacao where tpsstatus='A' and tpscod>9 and tpscod <13  order by tpscod ";
	  $db->monta_combo("tpscod",$sql,'S',"Escolha a Situação do Projeto",'','','Escolha a situação em que o projeto está ou deve estar.',400);
	?>
	</td>
      </tr>       
      <? if  ($pjeid) { ?> 


<tr bgcolor="#CCCCCC">
        <td></td>
        <td><?if ($autoriza)
             
        {   //Caso o usuário possa alterar faça
        	if ($alterar){?>
              <input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao"><?}?><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao"></td>
      </tr>      

<?} } else { ?>
      <tr bgcolor="#CCCCCC">
        <td></td>
        <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao"></td>
      </tr>
<? } ?>
   </table>



</tbody>
    </table>
    </center>
  </div>
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
<script>

   function validar_cadastro(cod)
   {

    prepara_formulario();
    if (!validaBranco(document.formulario.ungcod, 'Unidade Gestora')) return ;	    
	if (!validaBranco(document.formulario.pjecod, 'Código')) return;	
	if (!validaBranco(document.formulario.pjedsc, 'Título')) return;

	

	if (!validaData(document.formulario.pjedataini))
		{
		   alert("Data Inicio Inválida.");
		   document.formulario.pjedataini.focus();
		   return;
		}
	if (!validaData(document.formulario.pjedatafim))
		{
		   alert("Data Término Inválida.");
		   document.formulario.pjedatafim.focus();
		   return;
		}		
	
	if (!validaDataMaior(document.formulario.pjedataini , document.formulario.pjedatafim))
	{
		alert("Data Término não pode ser Anterior que Data Início.");
		document.formulario.pjedatafim.focus();
		return;
	}

	if (!validaBranco(document.formulario.pjedescricao, 'Descrição')) return;
	if (!validaBranco(document.formulario.pjefinalidade, 'Finalidade')) return;		
	if (cod == 'I') document.formulario.act.value = 'inserir'; 
	else 		document.formulario.act.value = 'alterar';
	
   	document.formulario.submit();
   }

  function ver_proj(cod) {
  	 document.formulario.ver.value = 1;
     document.formulario.pjeid.value = cod;
	 document.formulario.act.value = 'editar';      
     document.formulario.submit();
  }  
  
  function inclui_subacao(cod)
  {
  	e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/projespec/subacaopje&acao=I&cod="+cod;
         abreresp = window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=600'");	
  }

</script>
