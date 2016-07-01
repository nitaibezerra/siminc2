<?php 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:migradados.inc
   Finalidade: permitir migrar os dados de um ano de origem para um de destino
   */
// importação de responsabilidades
 $exercicio=$_SESSION['exercicio'];

/*
  pjeid int4,
  prgid int4,
  acaid int4,
  usucpf char(11),
  saoid int4,
  rpustatus char(1),
  rpudata_inc date,
  pflcod int4
  */
/*  if ($_REQUEST['resp2'])
{

	$sql="select * from monitora.tempa order by rpuid";
    $rs = @$db->carregar( $sql );
    if (  $rs && count($rs) > 0 )
    {  	
	   foreach ( $rs as $linha )
  	   {  		
		   foreach($linha as $k=>$v) ${$k}=$v;
		   $ok=0;	
		   $sql = "select rpuid from monitora.usuarioresponsabilidade where rpuid=$rpuid";
		   $ok=$db->pegaUm($sql);
		   if (! $ok)
		   {
		   	  // então insere
		   	  $sql="insert into monitora.usuarioresponsabilidade (rpustatus,rpuid,usucpf,rpudata_inc,acaid,pflcod) values('J',$rpuid,'$usucpf','$rpudata_inc',$acaid,$pflcod)";
              $db->executar($sql);
		   }
		   
		   
  	   }
  	   $db->commit();
    }
	
}
*/
if ($_REQUEST['resp'])
{
	
	// responsabilidades em programas
	$sql="select u.prgid,u.usucpf,u.pflcod from monitora.usuarioresponsabilidade u  inner join programa p on p.prgid=u.prgid and p.prgano='".$_REQUEST['anoorigem']."'  where u.prgid is not null and u.rpustatus='A'";
	$RS = $db->record_set($sql);

	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
			//dbg($sql,1);
		for ($i=0; $i<=$nlinhas;$i++)
   		{	unset($dados_destino);
	        unset($dados_origem);
     		$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql = "select prgcod from programa where prgid = $prgid";
     		$dados_origem= $db->recuperar($sql);
            $sql = "select prgid as novoprgid from programa where prgcod='".$dados_origem['prgcod']. "' and prgano='".$_REQUEST['anodestino']."'";
     		$dados_destino= $db->recuperar($sql);
            if ($dados_destino['novoprgid']>0)
            {
     		
     		// com os dados de destino, verifica-se se já ocorreu a migração para as responsabilidades na tabela de responsabilidades de usuário
     		$sql="select u.prgid from usuarioresponsabilidade u  where u.prgid=".$dados_destino['novoprgid']." and u.usucpf='$usucpf' and u.pflcod=$pflcod" ;
     		//print $sql;
     		$RS2 = $db->record_set($sql);
	        $nlinhas2 = $db->conta_linhas($RS2);
	        if ($nlinhas2 < 0)
	        {
	        	// então ainda não houve migração das responsabilidades em programa
	        	$sql="insert into usuarioresponsabilidade (prgid,usucpf,pflcod,prsano) values (".
	        	$dados_destino['novoprgid'].",'$usucpf',$pflcod,'$exercicio')";
	        	//dbg($sql);
	        	$saida=$db->executar($sql);
	        	
	        }
            }
   		}
   		$db->commit();

	}


	// responsabilidades em ações
	$sql="select u.acaid,u.usucpf,u.pflcod from usuarioresponsabilidade u  inner join acao a on a.acaid=u.acaid and a.prgano='".$_REQUEST['anoorigem']."'  where u.acaid is not null and u.rpustatus='A'";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	$mm=0;

	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			unset($dados_destino);
	        unset($dados_origem);
     		$res = $db->carrega_registro($RS,$i);
     		$dados_origem['acacod']=0;
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql = "select acacod,prgcod,unicod,prgano,acasnrap from acao where acasnrap='f' and acaid = $acaid";
     		$dados_origem= $db->recuperar($sql);
     		if ($dados_origem['acacod'])
     		{
                $sql = "select acaid as novoacaid from acao where acasnrap='f' and prgcod='".$dados_origem['prgcod']. "' and prgano='".$_REQUEST['anodestino']."' and acacod='".$dados_origem['acacod']."' and unicod='".$dados_origem['unicod']."'";
               $novoacaid=0;
               $RS2 = $db->record_set($sql);
	           $nlinhas2 = $db->conta_linhas($RS2);
			   if ($nlinhas2>=0)
				   for ($b=0;$b<=$nlinhas2;$b++)
				   {  
				      $res = $db->carrega_registro($RS2,$b);
                      if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                      if ($novoacaid > 0)
                      {     		
     		// com os dados de destino, verifica-se se já ocorreu a migração para as responsabilidades na tabela de responsabilidades de usuário
     		        $sql="select u.acaid from usuarioresponsabilidade u  where u.acaid=".$novoacaid." and u.usucpf='$usucpf' and u.pflcod=$pflcod" ;
     		        //print $sql;
     		              $RS3 = $db->record_set($sql);
	                      $nlinhas3 = $db->conta_linhas($RS3);
	                      if ($nlinhas3 < 0)
	                      {
							 
	        	        // então ainda não houve migração das responsabilidades em aç~çao
	        	             $sql="insert into monitora.usuarioresponsabilidade (acaid,usucpf,pflcod,prsano) values (".
	        	             $novoacaid.",'$usucpf',$pflcod,'$exercicio')";	     
	        	             $saida=$db->executar($sql);	        	
	                      }
                       }
				   }
				   $db->commit();
			}
   		}
    }
	
	//$sql = "insert into usuarioresponsabilidade (pjeid, prgid, acaid, usucpf, saoid, rpustatus, pflcod) select distinct u.pjeid, u.prgid, a2.acaid, u.usucpf, u.saoid, u.rpustatus, u.pflcod from usuarioresponsabilidade u inner join acao a on u.acaid=a.acaid and a.prgano='".$_REQUEST['anoorigem']."' and a.acasnrap='f'inner join acao a2 on a.prgcod=a2.prgcod and a.acacod=a2.acacod and a.unicod=a2.unicod and a.loccod=a2.loccod and a2.prgano='".$_REQUEST['anodestino']."' and a2.acasnrap='f' where rpustatus='A'";
//insert into usuarioresponsabilidade (pjeid, prgid, acaid, usucpf, saoid, rpustatus, pflcod) select distinct u.pjeid, p2.prgid, u.acaid, u.usucpf, u.saoid, u.rpustatus, u.pflcod from usuarioresponsabilidade u inner join programa p on p.prgid=u.prgid and p.prgano='2005' inner join programa p2 on p.prgcod=p2.prgcod and p2.prgano='2006' where rpustatus='A'
}
if ($_REQUEST['plantrab'])
{
// migrar plano de trabalho

//1 - procurar na tabela plantrabacao o acaid e o ptoid
// 2 - verificar na tabela planotrabalho se ou nao limite é menor ou igual ao ano destino da migração
//3 - descobrir na tabela acao o acacod, unicod, prgcod que corresponde aquele acaid
//4 - descobrir o acaid da mesma combinação no novo exercicio sem ser rap
//5 - dar um insert no plano de trabalho com o ptoid e o novo acaid 
    $sql="select p.acaid,p.ptoid from plantrabacao p where p.acaid in (select acaid from acao where prgano='".$_REQUEST['anoorigem']."')";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql = "select ptoanofim from planotrabalho where ptoid=$ptoid ";            
     		$dados_origem= $db->recuperar($sql);
     		if ($dados_origem['ptoanofim'] >=$_REQUEST['anodestino'])
     		{ 
     			
     			$sql = "select acacod,unicod,prgcod from acao where acaid=$acaid ";
     		    $dados_origem= $db->recuperar($sql);
     		    $sql = "select acaid as novoacaid from acao where acasnrap='f' and acacod='".$dados_origem['acacod']."' and prgcod='".$dados_origem['prgcod']."' and unicod='".$dados_origem['unicod']."' and prgano='".$_REQUEST['anodestino']."'";

     		    $dados_destino= $db->recuperar($sql);
     		    $RS2 = $db->record_set($sql);
	            $nlinhas2 = $db->conta_linhas($RS2);
     		            
	            if ($nlinhas2 >= 0)
	            {

	            	// entao existe o novoacaid
	            	// testo para ver se já foi migrado para o novo ano
	            	//$saida['acaid']=0;
	            	$sql="select acaid from plantrabacao where acaid=".$dados_destino['novoacaid']." and ptoid=$ptoid";
	            		    
	            	//print 'aqui estou'.$sql;
	            	$saida= $db->recuperar($sql);
	            	if (! $saida['acaid'])
	            	{
	        	        // então ainda não houve migração do plano de trabalho
	        	        $sql="insert into plantrabacao (acaid,ptoid) values (".$dados_destino['novoacaid'].",$ptoid)";
	        	        $saida=$db->executar($sql);	
	            	}
	        	            	
	             }
     		
     		}
   		}
   	}

}

$db->commit();
$db->sucesso($modulo);
?>
