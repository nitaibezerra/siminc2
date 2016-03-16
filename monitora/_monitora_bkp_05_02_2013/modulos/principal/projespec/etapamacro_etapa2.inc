<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:etapamacro_etapa2.inc
   finalidade: permitir o acompanhamento físico do projeto especial
   */
 
 
  
  $autoriza = false;
  $coordpje=false;
  $digit=false;
  $temdono=false;
  if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) {
  	$coordpje = true;
  	$status='coordpje';
  }
   // verifica se é digitador
  if ($db->testa_digitador($_SESSION['pjeid'],'E')) {
  	$digit = true;
  	$status='digit';
  }
// verific se é super-usuário
  if ($db->testa_superuser())  {
  	$coordpje = true; 
  	  $status='coordpje'; 
  }
  $temdono = $db->testa_proprietario($ptoid3);
  


    $sql = "select e.ptocod,e.ptodsc from monitora.planotrabalho e where e.ptoid=$ptoid3";
    $res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 

 
   $sql = "select e.ptoid as ptoidalt ,e.expid,e.exprealizado,e.expfinanceiro,e.expobs,to_char(e.expdata,'dd/mm/yyyy') as expdata,s.tpsdsc,u.usunome,e.usucpf as prop from monitora.execucaopje e inner join monitora.planotrabalho pt on pt.ptoid=e.ptoid and pt.ptostatus='A' and pt.ptosnaprovado='t' inner join tiposituacao s on s.tpscod=e.tpscod inner join seguranca.usuario u on u.usucpf=e.usucpf where e.ptoid=$ptoid3 order by e.expdata";
    	print "<tr align='left'><td colspan='6'><b>$ptocod - $ptodsc</b></td></tr>";

  
  $rs = @$db->carregar( $sql );
  if (  $rs && count($rs) > 0 )
  {
  	print "<tr align='left'><td></td><td align='left' ><b>Físico</b></td><td><b>Financeiro</b></td><td><b>Situação</b></td><td><b>Responsável</b></td><td><b>Data</b></td></tr>";
	$i=0;
	foreach ( $rs as $linha )
	{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 
		
		 
		 if (($coordpje) and ($temdono) and ($prop==$_SESSION['usucpf']))
  { 
	     $linha = '<tr align="left"><td><img border="0" src="../imagens/alterar.gif" title="Alterar acompanhamento." onclick="edita_acomp('."'".$ptoid3."','".$expid."'".')">&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir o acompanhamento." onclick="excluir_acomp('."'".$ptoid3."','".$expid."'".')"></td>'."<td align='left' >$exprealizado</td><td>$expfinanceiro</td><td>$tpsdsc</td><td>$usunome</td><td>$expdata</td></tr>";
         
  }
  else 
  {
  		     $linha = '<tr align="left"><td></td>'."<td align='left' >$exprealizado</td><td>$expfinanceiro</td><td>$tpsdsc</td><td>$usunome</td><td>$expdata</td></tr>";
  }
         print $linha;
         $i++;
	}
  }
  if (($coordpje or $digit) and ($temdono) and projetoaberto() )
  {
  	
  ?>
      <tr align='left'>
      <td align='left' colspan='6'>
         <input type='button' value="Incluir dados" onclick="insere_acomp('<?=$ptoid3?>')"></td>

         </tr>
         <?}?>





