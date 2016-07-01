<?php
include "resultSession.php";

function obterSqlGrafico($anoEspecifico = null) {
	
	if( $anoEspecifico == 2008 ){
		$where = " WHERE escolapse.espparticipapse2009 = 't'";	
	}elseif( $anoEspecifico == 2009 ){
		$where = "WHERE escolapse.espparticipapse2009 = 'f'";	
	}else{
		$where = "WHERE escolapse.espparticipapse2009 IN ('f','t')";	
	}
	
	
	$condicaoParticipaPse2009 = $anoEspecifico==2008 ? 't' : 'f';
		// 2008 --> 't'
		// 2009 --> 'f' 	
		
	if( $anoEspecifico <> 10 ){
		$select = $anoEspecifico." as ano,";
		$group = ", ano";		
	}
	
	$sql = "
			select distinct
			  endereco.estuf 	      	  as ufb   , 
			  ".$select." 
			  count(escolas_ano_especifico.entid) as escolas_atendidas , 
			  escolas_2008_e_2009.total   as total
			
			from  
			  ( select 
				* 
			    from 
				pse.escolapse as escolapse
			        ".$where."	
			    ) as escolas_ano_especifico,		
			   		entidade.endereco as endereco ,
			
			  ( select 
				enderecob.estuf 	as ufc   , 
				count(escolapse.entid) 	as total 
			    from 
				pse.escolapse as escolapse
			
				inner join entidade.endereco as enderecob 
				on escolapse.entid = enderecob.entid
			    
			    group by 
				ufc
			  ) as escolas_2008_e_2009 
			
			where 
			  escolas_ano_especifico.entid = endereco.entid
			  and 
			  endereco.estuf = escolas_2008_e_2009.ufc
			
			group by
			  ufb 
			  ".$group." 
			  ,total
			ORDER BY
				endereco.estuf";
	return $sql;
}

function retornaGrafico ( $titulo, $dados ){
	
	
	$title = new title( removeacentosGrafico($titulo) );
	$title->set_style( "{font-size: 20px; color: #F24062; text-align: center;}" );
	
	$bar_stack = new bar_stack();
	$bar_stack->set_colours( array( '#C4D318', '#50284A' ) );
		
	foreach( $dados as $linhaRegistro ){
		$quantiaEscolasAtendidas = (int) $linhaRegistro['escolas_atendidas'];
		$quantiaTotalEscolas = (int) $linhaRegistro['total'];
		$quantiaDiferencaAtendidasTotal = $quantiaTotalEscolas - $quantiaEscolasAtendidas;  
		$bar_stack->append_stack( array( $quantiaEscolasAtendidas, $quantiaDiferencaAtendidasTotal ) );
		
		$arrMax[] =  $quantiaEscolasAtendidas + $quantiaDiferencaAtendidasTotal;
	}
	
	$bar_stack->set_tooltip( '#x_label#' );
	
	$y = new y_axis();
	$y->set_range( 0, (max($arrMax) + 50), 50 );
	
	foreach( $dados as $linhaRegistro ){
		$arrayRotulacaoUf[] = $linhaRegistro['ufb'];
	}
	
	$x = new x_axis();
	$x->set_labels_from_array( $arrayRotulacaoUf );	
	
	
	$tooltip = new tooltip();
	$tooltip->set_hover();
	
	$chart = new open_flash_chart();
	$chart->set_title( $title );
	$chart->add_element( $bar_stack );
	$chart->set_x_axis( $x );
	$chart->add_y_axis( $y );
	$chart->set_tooltip( $tooltip );
	
	return $chart->toPrettyString();
	
}

function removeacentosGrafico ($var)
{
       $ACENTOS   = array("À","Á","Â","Ã","à","á","â","ã");
       $SEMACENTOS= array("A","A","A","A","a","a","a","a");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("È","É","Ê","Ë","è","é","ê","ë");
       $SEMACENTOS= array("E","E","E","E","e","e","e","e");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ì","Í","Î","Ï","ì","í","î","ï");
       $SEMACENTOS= array("I","I","I","I","i","i","i","i");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("Ò","Ó","Ô","Ö","Õ","ò","ó","ô","ö","õ");
       $SEMACENTOS= array("O","O","O","O","O","o","o","o","o","o");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
     
       $ACENTOS   = array("Ù","Ú","Û","Ü","ú","ù","ü","û");
       $SEMACENTOS= array("U","U","U","U","u","u","u","u");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ç","ç","ª","º","°");
       $SEMACENTOS= array("C","c","a.","o.","o.");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);      

       return $var;
}

?>